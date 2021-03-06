#include"MyPolygon.h"
#include"DxLib.h"
#include<cassert>
#include<exception>
#include<list>
#include<array>
#include<iterator>
#include"Circle.h"
#include"Edge.h"

//--------------------MyPolygon--------------------
MyPolygon::MyPolygon(Vector2D begin,std::vector<Vector2D> points,Fix::Kind fix)
	:Shape(begin,Type::e_polygon,fix)
{
	//m_edgeVecsの初期化
	size_t size=points.size();
	if(size<2){
		//合計頂点数が3を下回ったら多角形では無いので例外を飛ばす
		throw std::invalid_argument("You make polygon with "+std::to_string(size+1)+" points.");
	}
	Vector2D edgevec=points.at(0)-m_position;
	m_edgeVecs.push_back(edgevec);
	for(size_t i=0;i+1<size;i++){
		edgevec=points.at(i+1)-points.at(i);
		m_edgeVecs.push_back(edgevec);
	}
	//m_triangleSetの初期化
	DivideTriangle(&m_triangleSet);
}

MyPolygon::~MyPolygon(){}

std::vector<Vector2D> MyPolygon::GetAllEdgeVecs()const{
	std::vector<Vector2D> ret=m_edgeVecs;
	Vector2D addVec(0.0f,0.0f);
	for(const Vector2D &e:m_edgeVecs){
		addVec-=e;
	}
	ret.push_back(addVec);
	return ret;
}

std::shared_ptr<Shape> MyPolygon::VCopy()const{
	return std::shared_ptr<Shape>(new MyPolygon(*this));
}

void MyPolygon::DivideTriangle(std::vector<std::array<size_t,3>> *triangleSet)const{
	//使う関数
	auto FuncNextIt=[](std::list<std::pair<Vector2D,size_t>>::iterator it,std::list<std::pair<Vector2D,size_t>> &list){
		it++;
		if(it==list.end()){
			it=list.begin();
		}
		return it;
	};
	auto FuncBackIt=[](std::list<std::pair<Vector2D,size_t>>::iterator it,std::list<std::pair<Vector2D,size_t>> &list){
		if(it==list.begin()){
			it=list.end();
		}
		it--;
		return it;
	};
	//三角形分割を行い、分割できた全ての三角形について内部判定処理を行う
	triangleSet->clear();
	//三角形分割初期化
	std::list<std::pair<Vector2D,size_t>> vertexList={std::pair<Vector2D,size_t>(m_position,0)};//消していない頂点一覧。初期値はPolygonの全ての頂点。リストの中を削除する行為が多いので双方向リストの方が趣旨に合っている。
	for(size_t i=0,size=m_edgeVecs.size();i<size;i++){
		vertexList.push_back(std::pair<Vector2D,size_t>(vertexList.back().first+m_edgeVecs[i],i+1));
	}
	size_t leftCount=vertexList.size();//残り個数
	//三角形分割実行
	while(leftCount>3){
		//残り頂点数が3個以下になるまで頂点を1つずつ取り除く
		//原点から最も遠い点を取り出す
		std::list<std::pair<Vector2D,size_t>>::iterator baseIt=vertexList.begin();
		for(std::list<std::pair<Vector2D,size_t>>::iterator it=vertexList.begin(),ite=vertexList.end();it!=ite;it++){
			if(it->first.sqSize()>baseIt->first.sqSize()){
				baseIt=it;
			}
		}
		//その点と隣接する2点による三角形について、外積を求めておく
		//最遠点を基準とする三角形は必ず多角形内部を含むため、三角形の向きを示す外積の向きを用いれば、「三角形が多角形の外部に存在する」というパターンを弾ける。
		std::list<std::pair<Vector2D,size_t>>::iterator backIt,nextIt;
		const float baseCross=((FuncBackIt(baseIt,vertexList)->first)-(baseIt->first)).cross((FuncNextIt(baseIt,vertexList)->first)-(baseIt->first));
		//最遠点からbaseItを++させる方向に順番に三角形分割できるか見ていく
		while(true){
			//三角形の向きの条件をクリアしているか
			const float cross=((FuncBackIt(baseIt,vertexList)->first)-(baseIt->first)).cross((FuncNextIt(baseIt,vertexList)->first)-(baseIt->first));
			if(cross*baseCross>0.0f){
				//三角形内部に他の多角形点がないかを判定
				std::list<std::pair<Vector2D,size_t>>::iterator it=FuncNextIt(FuncNextIt(baseIt,vertexList),vertexList);
				bool noexist=true;
				for(;it!=FuncBackIt(baseIt,vertexList);it=FuncNextIt(it,vertexList)){
					if(JudgeInTriangle(it->first,FuncBackIt(baseIt,vertexList)->first,baseIt->first,FuncNextIt(baseIt,vertexList)->first)){
						//外積の符号がどれも同じなら点が内部にあるので、内部にある判定をしてforループ終了
						noexist=false;
						break;
					}
				}
				if(noexist){
					//三角形分割できる場合はループを脱出
					triangleSet->push_back({FuncBackIt(baseIt,vertexList)->second,baseIt->second,FuncNextIt(baseIt,vertexList)->second});
					vertexList.erase(baseIt);
					leftCount--;
					break;
				}
			}
			//分割できない場合は次の点へ
			baseIt=FuncNextIt(baseIt,vertexList);
		}
	}
	//最後に余った3点の三角形を格納
	std::array<size_t,3> tri;
	std::list<std::pair<Vector2D,size_t>>::iterator it=vertexList.begin();
	for(size_t i=0;i<3;i++,it++){
		tri[i]=it->second;
	}
	triangleSet->push_back(tri);
}

void MyPolygon::CalculateAllPointPosition(std::vector<Vector2D> *vertexPos)const{
	vertexPos->clear();
	vertexPos->reserve(m_edgeVecs.size()+1);//高速化のため
	vertexPos->push_back(m_position);
	Vector2D v=m_position;
	for(const Vector2D &vec:m_edgeVecs){
		v+=vec;
		vertexPos->push_back(v);
	}
}

void MyPolygon::Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	//中を満たすかどうかで処理を変える。満たさない場合は三角形分割をしなくて良いので高速化できるため
	if(fillFlag==TRUE){
		//中を満たす場合
		//三角形分割
		//std::vector<std::array<size_t,3>> triangleSet;
		//DivideTriangle(&triangleSet);

		//分割した三角形の集合ができたので、全ての三角形に対して内部判定を行っていく
		//再利用性を考えて、前では頂点番号で三角形集合を表現して、こちらでまた座標一覧を作っていく
		std::vector<Vector2D> vertexVec;
		CalculateAllPointPosition(&vertexVec);
		//全ての分割三角形に対して、内部判定を実行
		unsigned int c;
		//c=VJudgePointInsideShape(GetMousePointVector2D())?GetColor(168,128,128):GetColor(128,168,128);
		c=color;
		for(size_t i=0,size=m_triangleSet.size();i<size;i++){
			DrawTriangleAA(vertexVec[m_triangleSet[i][0]].x+adjust.x,vertexVec[m_triangleSet[i][0]].y+adjust.y,
				vertexVec[m_triangleSet[i][1]].x+adjust.x,vertexVec[m_triangleSet[i][1]].y+adjust.y,
				vertexVec[m_triangleSet[i][2]].x+adjust.x,vertexVec[m_triangleSet[i][2]].y+adjust.y,
				c,TRUE,lineThickness);
			DrawTriangleAA(vertexVec[m_triangleSet[i][0]].x+adjust.x,vertexVec[m_triangleSet[i][0]].y+adjust.y,
				vertexVec[m_triangleSet[i][1]].x+adjust.x,vertexVec[m_triangleSet[i][1]].y+adjust.y,
				vertexVec[m_triangleSet[i][2]].x+adjust.x,vertexVec[m_triangleSet[i][2]].y+adjust.y,
				color,FALSE,lineThickness);
		}

	} else{
		//辺のみ描画する場合
		Vector2D pos=point+adjust,next;
		for(const Vector2D &edge:GetAllEdgeVecs()){
			//始点からもう一度始点に戻るまでの全ての辺の描画
			next=pos+edge;
			DrawLineAA(pos.x,pos.y,next.x,next.y,color,lineThickness);
			pos=next;
		}
	}
}

bool MyPolygon::PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const{
	//押し出し距離の計算はできないが、辺が全て線分なので全ての辺をEdgeクラスにしそれとの押し出し処理を行わせる
	bool ret=false;
	Vector2D begin=m_position;
	for(const Vector2D &vec:GetAllEdgeVecs()){
		//全ての線分に対しての処理
		Edge e(begin,vec,this->m_fix);
		ret=e.PushParentObj(pShape,parentObj,pushRate) | ret;//線分による押し出し処理+当たったかどうかの更新処理
		begin+=vec;
	}
	return ret;
}

bool MyPolygon::JudgeInShape(const Shape *pShape)const{
	//図形に応じて最適な内部判定アルゴリズムを適用する
	switch(pShape->GetType()){
	case(Shape::Type::e_circle):
		//円との内部判定は、すべての分割三角形について「三角形内部に中心が存在する」「３辺と円が交点を持つ＝押し出し距離が0より大きい」のどちらも満たさないかどうかで行う
		{
			const Circle *c=dynamic_cast<const Circle *>(pShape);
			if(c!=nullptr){
				std::vector<Vector2D> pointPos;
				CalculateAllPointPosition(&pointPos);
				for(const std::array<size_t,3> triangle:m_triangleSet){
					//3つのEdgeを作る
					Edge edges[3]={Edge(pointPos[triangle[0]],pointPos[triangle[1]],pShape->m_fix),Edge(pointPos[triangle[1]],pointPos[triangle[2]],pShape->m_fix),Edge(pointPos[triangle[2]],pointPos[triangle[0]],pShape->m_fix)};
					if(!JudgeInTriangle(c->GetPosition(),pointPos[triangle[0]],pointPos[triangle[1]],pointPos[triangle[2]])){
						//中心が三角形外にある場合は辺と円の当たり判定をする
						for(size_t i=0;i<3;i++){
							c->JudgeInShape(&edges[i]);
						}
					} else{
						//中心が三角形内にある場合は多角形内部に円があると見なせる
						return true;
					}
				}
			} else{
				//ここに来ることはないはず
				assert(false);
			}
		}
		break;
	case(Shape::Type::e_edge):
		//直線との内部判定は「始点と終点がどちらも多角形外」「どの辺ともedgeが交差しない」を調べれば良い
		//使わないから実装しない
		break;
	}
	return false;
}

Vector2D MyPolygon::GetLeftTop()const{
	Vector2D leftTop=m_position,point=m_position;
	for(const Vector2D &edge:m_edgeVecs){
		//全ての頂点について最小値を求める
		point+=edge;
		leftTop.x=std::fminf(leftTop.x,point.x);
		leftTop.y=std::fminf(leftTop.y,point.y);
	}
	return leftTop;
}

Vector2D MyPolygon::GetRightBottom()const{
	Vector2D rightBottom=m_position,point=m_position;
	for(const Vector2D &edge:m_edgeVecs){
		//全ての頂点について最大値を求める
		point+=edge;
		rightBottom.x=std::fmaxf(rightBottom.x,point.x);
		rightBottom.y=std::fmaxf(rightBottom.y,point.y);
	}
	return rightBottom;
}

void MyPolygon::RecordLatticePointInShape(std::vector<int> &latticeInShape,const size_t xNum,const size_t yNum,const size_t squareWidth,const size_t squareHeight,int index)const{
	//三角形分割を行い、分割できた全ての三角形について格子点検出を行う
	//これだと凸多角形にしか適用できなくない？→MyPolygon初期化時に多角形を三角形分割していて、その結果(m_triangleSet)を用いているので大丈夫。
	std::vector<Vector2D> vertexVec;
	CalculateAllPointPosition(&vertexVec);
	for(const std::array<size_t,3> &indexVec:m_triangleSet){
		//格子点を求める処理を行う
		//三角形頂点をxが小さい順に列挙
		Vector2D p[3]={vertexVec[indexVec[0]],vertexVec[indexVec[1]],vertexVec[indexVec[2]]};//p[3]には、三角形3点がxの小さい順に格納される
		if(p[0].x>p[1].x){
			Vector2D v=p[0];
			p[0]=p[1];
			p[1]=v;
		}
		if(p[1].x>p[2].x){
			Vector2D v=p[1];
			if(p[0].x<=p[2].x){
				p[1]=p[2];
				p[2]=v;
			} else{
				p[1]=p[0];
				p[0]=p[2];
				p[2]=v;
			}
		}
		if(p[2].x-p[0].x==0.0f){
			//3点が一直線上にあったら何もしない
			continue;
		}
		//探索用indexの設定(x基準)
		const size_t xMin=(size_t)ceilf(std::fmax(p[0].x,0.0f)/(float)squareWidth),xCenter=(size_t)std::fmax(p[1].x,0.0f)/squareWidth,xMax=(size_t)std::fmax(p[2].x,0.0f)/squareWidth;
		//格子点配列の要素の変更
		const Vector2D vertex[3]={p[1]-p[0],p[2]-p[1],p[2]-p[0]};
		for(size_t x=xMin;x<=xMax;x++){
			size_t yIndex[2]={};//変更する格子点の最大最小
			const float pointX=(float)(x*squareWidth);
			//直線x=pointXが三角形と交わる点を求める
			float pointY[2];
			if(x<=xCenter){
				//真ん中の点より左側の時(xCenterは切り下げで求めているので、x==xCenterの時もこちらを用いる)
				if(vertex[0].x!=0.0f){
					//p[0],p[1]がy軸平行線上にない場合
					pointY[0]=(p[0].y*(p[2].x-pointX)+p[2].y*(pointX-p[0].x))/vertex[2].x;
					pointY[1]=(p[0].y*(p[1].x-pointX)+p[1].y*(pointX-p[0].x))/vertex[0].x;
				} else{
					//p[0],p[1]がy軸平行線上にある場合
					pointY[0]=p[0].y;
					pointY[1]=p[1].y;
				}
			} else{
				//真ん中の点より右側の時
				if(vertex[1].x!=0.0f){
					//p[2],p[1]がy軸平行線上にない場合
					pointY[0]=(p[0].y*(p[2].x-pointX)+p[2].y*(pointX-p[0].x))/vertex[2].x;
					pointY[1]=(p[1].y*(p[2].x-pointX)+p[2].y*(pointX-p[1].x))/vertex[1].x;
				} else{
					//p[0],p[1]がy軸平行線上にある場合
					pointY[0]=p[2].y;
					pointY[1]=p[1].y;
				}
			}
			//交点y座標をソート
			if(pointY[0]>pointY[1]){
				float f=pointY[0];
				pointY[0]=pointY[1];
				pointY[1]=f;
			}
			//格子点の要素変更
			if(pointY[1]>=0.0f){
				const size_t yMin=(size_t)ceilf(std::fmax(pointY[0],0.0f)/(float)squareHeight);
				size_t yMax=(size_t)pointY[1]/squareWidth;
				if(yMax>=yNum){
					yMax=yNum-1;
				}
				for(size_t y=yMin;y<=yMax;y++){
					//ここに来れば無条件で三角形内部
					latticeInShape[x+y*xNum]=index;
				}
			}
		}
	}

}

bool MyPolygon::VJudgePointInsideShape(Vector2D point)const{
	//三角形分割を行い、分割できた全ての三角形について内部判定処理を行う
	std::vector<Vector2D> vertexVec;
	CalculateAllPointPosition(&vertexVec);
	for(const std::array<size_t,3> &index:m_triangleSet){
		if(JudgeInTriangle(point,vertexVec[index[0]],vertexVec[index[1]],vertexVec[index[2]])){
			//どれか1つの三角形に入っていれば多角形内部に入っている
			return true;
		}
	}

	return false;
}

Vector2D MyPolygon::VGetNearEndpoint(Vector2D point,float capacity)const{
	//全ての頂点について近いかどうかを判定する
	Vector2D vertex=m_position;
	const float sqcapacity=capacity*capacity;
	for(const Vector2D &edge:GetAllEdgeVecs()){
		//始点の次の点から探索し、最後は始点を判定する
		vertex+=edge;
		if((point-vertex).sqSize()<=sqcapacity){
			//近い端点を見つけたらその座標を返す
			return vertex;
		}
	}
	//どの頂点にも近くないならpointを返す
	return point;
}

void MyPolygon::Resize(Vector2D v){
	//MyPolygonの1つ目の辺がvとなるように、拡大縮小だけでなく回転もできるようにする。
	/*m_edgeVecs[0]とvを比較し、全てのm_edgeVecsに対してm_edgeVecs[0]からvへの拡大率と回転角度を適用する
	//変換行列をそのまま求めた方が高速なのでそれを計算しResizeする。
	//v2=Av1となるAは次のように計算できる。(v1=(x1,y1),v2=(x2,y2))
	//A=(a,-b)
	//  (b,a) (a=(x1x2+y1y2)/(x1^2+y1^2),b=(x1y2-x2y1)/(x1^2+y1^2))
	//*/
	Vector2D v1=m_edgeVecs.at(0);
	const float a=(v1.x*v.x+v1.y*v.y)/(v1.x*v1.x+v1.y*v1.y)
		,b=(v1.x*v.y-v.x*v1.y)/(v1.x*v1.x+v1.y*v1.y);
	for(Vector2D &edgeVec:m_edgeVecs){
		//各辺に対して回転行列を適用
		edgeVec.x=a*edgeVec.x-b*edgeVec.y;
		edgeVec.y=b*edgeVec.x+a*edgeVec.y;
	}
}

Vector2D MyPolygon::GetRetResize()const{
	//完全に現在の形を再現することはできない。拡大率と回転角度を一致させるだけなら最初の辺をvとして用いればよい。
	return m_edgeVecs[0];
}

void MyPolygon::WriteOutShape(std::ofstream &ofs)const{
	//"("→(種別)→(始点位置)→(初期固定)→((各辺の方向ベクトル)→...)→")"
	ofs<<beginer<<Type::GetStr(m_type)<<spliter;//ofs<<"(Polygon,";
	ofs<<beginer<<m_position.x<<spliter<<m_position.y<<ender<<spliter;//ofs<<"("<<m_position.x<<","<<m_position.y<<"),";
	ofs<<Fix::GetStr(m_fix)<<spliter;//ofs<<Fix::GetStr(m_fix)<<",";
	//全ての辺の座標を()で全体を囲ってから出力
	ofs<<beginer;//ofs<<"("
	//始点を除いた頂点座標を全て出力
	Vector2D vertex=m_position;
	for(size_t i=0,size=m_edgeVecs.size();i<size;i++){
		//頂点座標の計算
		vertex+=m_edgeVecs.at(i);
		//座標を(x,y)の形式で出力
		ofs<<beginer<<vertex.x<<spliter<<vertex.y<<ender;//ofs<<"(x,y)"
		if(i+1!=size){
			ofs<<spliter;//最後の辺でなければ区切り文字を出力する
		}
	}
	ofs<<ender<<ender;//ofs<<"))";	
}

MyPolygon MyPolygon::CreateRectangle(Vector2D begin,Vector2D offset,Fix::Kind fix){
	std::vector<Vector2D> points={begin+Vector2D(offset.x,0.0f),begin+offset,begin+Vector2D(0.0f,offset.y)};
	return MyPolygon(begin,points,fix);
}