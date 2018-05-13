#include"MyPolygon.h"
#include"DxLib.h"
#include<cassert>
#include<exception>

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

void MyPolygon::Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	//中を満たすかどうかで処理を変える。満たさない場合は三角形分割をしなくて良いので高速化できるため
	if(fillFlag==TRUE){
		//中を満たす場合
		Vector2D pos=point+adjust,next;
		for(const Vector2D &edge:GetAllEdgeVecs()){
			//始点からもう一度始点に戻るまでの全ての辺の描画
			next=pos+edge;
			DrawLineAA(pos.x,pos.y,next.x,next.y,GetColor(168,128,128),lineThickness);
			pos=next;
		}
	} else{
		//辺のみ描画する場合
		Vector2D pos=point+adjust,next;
		for(const Vector2D &edge:GetAllEdgeVecs()){
			//始点からもう一度始点に戻るまでの全ての辺の描画
			next=pos+edge;
			DrawLineAA(pos.x,pos.y,next.x,next.y,GetColor(168,128,128),lineThickness);
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
	//三角形分割を行い、分割できた全ての三角形について内部判定処理を行う

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

bool MyPolygon::VJudgePointInsideShape(Vector2D point)const{
	//三角形分割を行い、分割できた全ての三角形について内部判定処理を行う

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
