#include"Edge.h"
#include"Circle.h"
#include"MyPolygon.h"
#include"DxLib.h"
#include<cassert>
#include"ToolsLib.h"

//--------------------Edge--------------------
Edge::Edge(Vector2D begin,Vector2D vec,Fix::Kind fix)
	:Shape(begin,Type::e_edge,fix),m_vec(vec){}

Edge::~Edge(){}

std::shared_ptr<Shape> Edge::VCopy()const{
	return std::shared_ptr<Shape>(new Edge(this->m_position,this->m_vec,this->m_fix));
}

Vector2D Edge::CalculateParpendicularVec(Vector2D point)const{
	//pointからこの直線に垂線を下ろした時、直線からpointに向けた方向の垂線ベクトルを求める
	float dot=m_vec.dot(point-GetBeginPoint());//垂線の足の位置はm_position+Norm()*dotとなる
	Vector2D parpendicularPoint=m_position+m_vec.norm()*dot;//垂線の足の位置
	return parpendicularPoint-point;
}

void Edge::Draw(Vector2D point,Vector2D adjust,float exRate,unsigned int color,int fillFlag,float lineThickness)const{
	Vector2D begin=point*exRate+adjust;
	Vector2D end=begin+m_vec*exRate;
	DrawLineAA(begin.x,begin.y,end.x,end.y,color,lineThickness*exRate);
}

Vector2D Edge::CalculatePushVec(const Shape *pShape)const{
	Vector2D ret;
	//pShapeの種類によって処理方法が変わる
	switch(pShape->GetType()){
	case Type::e_circle:
		{
			//pShapeをCircleにキャストする
			const Circle *pcircle=dynamic_cast<const Circle *>(pShape);
			if(pcircle!=nullptr){
				//直線と円は円の中心が直線に下ろした垂線の足の位置を用いて判定可能
				//円の中心から線分への最近傍点を求める
				float dot=m_vec.dot(pcircle->GetPosition()-GetBeginPoint());//垂線の位置はm_position+Norm()*dot/Size()となる
				Vector2D nearestPoint;//円の中心に最も近い線分上の点
				if(dot<0.0f){
					//線分外に垂線の足があり、最近傍点がm_positionになる
					nearestPoint=GetBeginPoint();
				} else if(dot<=m_vec.sqSize()){
					//線分内に垂線の足がある場合。
					nearestPoint=m_position+m_vec*dot/m_vec.sqSize();
				} else{
					//線分外に垂線の足があり、最近傍点がm_position+m_vecになる
					nearestPoint=GetEndPoint();
				}
				//最近傍点と円の当たり判定を行う。円から最近傍点を押し出す。
				Vector2D hVec=nearestPoint-pcircle->GetPosition();//円中心から最近傍点へのベクトル
				float pushLength=pcircle->GetR()-hVec.size();
				if(pushLength>0.0f){
					//最近傍点と円の距離が半径に等しくなるように押し出しベクトルを定義
					ret=hVec.norm()*pushLength;
				} else{
					//pushLength<0なら押し出ししない
					ret=Vector2D();
				}
			} else{
				//ここに来ることは無いはず。取り敢えず当たってないことにする。
				assert(false);
				ret=Vector2D();
			}
		}
	break;
	case Type::e_edge:
		{
			//pShapeをEdgeにキャストする
			const Edge *pedge=dynamic_cast<const Edge *>(pShape);
			if(pedge!=nullptr){
				//この実装では線と線の当たり判定はできない。なお、実装するなら移動前後の位置が分かる必要がある。
				//ひとまず当たったかどうかの判定ができるように、Vector2D(0.001f,0.001f)だけ押し出すことにする。
				if(this->m_vec!=pedge->m_vec){
					//平行でない場合
					//連立方程式を解けば良い
					//使わないので実装しない
				} else{
					//平行である場合
					//使わないので実装しない
				}
			} else{
				//ここに来ることは無いはず。取り敢えず当たってないことにする。
				assert(false);
				ret=Vector2D();
			}
		}
		break;
	default:
		ret=Vector2D();
		break;
	}
	return ret;
}

void Edge::BrokenDraw(Vector2D adjust,unsigned int color,float lineThickness,float drawSize,float eraseSize)const{
	BrokenDraw(GetPosition(),adjust,1.0f,color,lineThickness,drawSize,eraseSize);
}

void Edge::BrokenDraw(Vector2D point,Vector2D adjust,float exRate,unsigned int color,float lineThickness,float drawSize,float eraseSize)const{
	Vector2D begin=point*exRate+adjust;
	Vector2D end=begin+m_vec*exRate;
	float thickness=lineThickness*exRate;
	if(thickness<1.0f){
		thickness=1.0f;
	}
	DrawBrokenLineAA(begin,end,color,thickness,drawSize,eraseSize);
}

bool Edge::PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const{
	//自分とぶつかった相手の候補pShapeが何かによって押し出し処理が異なる
	switch(pShape->GetType()){
	case(Type::e_circle):
	case(Type::e_edge):
		{
			//相手が円か線分の場合は押し出し距離が求められるので、それをそのまま使う
			//押し出し距離の計算
			Vector2D pushVec=this->CalculatePushVec(pShape);
			//押し出し距離の一定の割合のベクトルだけ押し出す
			parentObj->Move(pushVec*pushRate);
			//押し出し距離の長さでtrueかfalseかを判断して返す
			return pushVec.sqSize()!=0.0f;
		}
	case(Type::e_polygon):
		{
			//相手が多角形の場合は、線分に分解して処理を行う
			const MyPolygon *ppolygon=dynamic_cast<const MyPolygon *>(pShape);
			if(ppolygon!=nullptr){
				bool ret=false;
				Vector2D begin=ppolygon->GetPosition();
				for(const Vector2D &vec:ppolygon->GetAllEdgeVecs()){
					//全ての線分に対しての処理
					Edge e(begin,vec,ppolygon->m_fix);
					ret=e.PushParentObj(pShape,parentObj,pushRate) | ret;//線分による押し出し処理+当たったかどうかの更新処理
					begin+=vec;
				}
				return ret;
			} else{
				//ここに来ることがあったら、なんかバグってる
				assert(false);
			}
		}
	}
	return false;
}

bool Edge::JudgeInShape(const Shape *pShape)const{
	//線分の場合は押し出し距離を求められるのでそれを用いる
	return CalculatePushVec(pShape)!=Vector2D();
}

bool Edge::JudgeCross(const Shape *pShape)const{
	const Type::Kind kind=pShape->GetType();
	if(kind==Type::e_circle){
		const Circle *pCircle=dynamic_cast<const Circle *>(pShape);
		if(pCircle!=nullptr){
			//直線と円の交差判定は、円の中心から直線上の点の距離が円の半径Rより大きい・小さいが切り替わるかどうかで判定できる
			//円の中心から線分への最近傍点がどこかで場合分けする
			const Vector2D center=pCircle->GetPosition();
			const float dot=m_vec.dot(center-GetBeginPoint());//垂線の位置はm_position+Norm()*dot/Size()となる
			float sqR=pCircle->GetR();
			sqR*=sqR;
			if(dot<0.0f){
				//線分外に垂線の足があり、最近傍点がm_positionになる
				//2端点と円の中心の距離の関係を求めれば良い
				const float nearestSqLength=(GetBeginPoint()-center).sqSize();
				const float farestSqLength=(GetEndPoint()-center).sqSize();
				return (nearestSqLength-sqR)*(farestSqLength-sqR)<=0.0f;
			} else if(dot<=m_vec.sqSize()){
				//線分内に垂線の足がある場合。
				//「垂線の足と1つの端点の距離関係」と「垂線の足ともう1つの端点の距離関係」の２つを調べれば良い
				const float beginSqLength=(GetBeginPoint()-center).sqSize();
				const float vecSqSize=m_vec.sqSize();
				const float nearestSqLength=(vecSqSize*beginSqLength-dot*dot)/vecSqSize;
				const float endSqLength=(GetEndPoint()-center).sqSize();
				return ((nearestSqLength-sqR)*(beginSqLength-sqR)<=0.0f || (nearestSqLength-sqR)*(endSqLength-sqR)<=0.0f);
			} else{
				//線分外に垂線の足があり、最近傍点がm_position+m_vecになる
				//2端点と円の中心の距離の関係を求めれば良い
				const float farestSqLength=(GetBeginPoint()-center).sqSize();
				const float nearestSqLength=(GetEndPoint()-center).sqSize();
				return (nearestSqLength-sqR)*(farestSqLength-sqR)<=0.0f;
			}
		}
	} else if(kind==Type::e_edge){
		const Edge *pEdge=dynamic_cast<const Edge *>(pShape);
		if(pEdge!=nullptr){
			//this->m_position=(x1,y1),this->m_vec=(x2,y2),pEdge->m_position=(x3,y3),pEdge->m_vec=(x4,y4)と置く。
			//(x2*y4-x4*y2)==0の時傾きが0なので平行である、この値は後で用いる
			const float inclination=this->m_vec.x*pEdge->m_vec.y-pEdge->m_vec.x*this->m_vec.y;
			if(inclination==0.0f){
				//平行の場合
				//同一直線上に存在するかの判定をする(2つのposition同士を結んだ直線とthisが平行かどうかで判定すればよい)
				//p1=this->GetBeginPoint(),p2=this->GetEndPoint(),q1=pEdge->GetBeginPoint(),q2=pEdge->GetEndPoint()である
				const Vector2D p1p2=pEdge->GetBeginPoint()-this->GetBeginPoint();
				if(m_vec.x*p1p2.y==p1p2.x*m_vec.y){
					//同一直線上にあった場合、内積を用いて判定
					const Vector2D p1q2=pEdge->GetEndPoint()-this->GetBeginPoint();
					const Vector2D q1p2=pEdge->GetBeginPoint()-this->GetEndPoint();
					const Vector2D q1q2=pEdge->GetEndPoint()-this->GetEndPoint();
					if(p1p2.dot(m_vec)<0.0f && p1q2.dot(m_vec)<0.0f){
						//pEdgeの両端点がthisのbeginよりm_vecと逆側にある時
						return true;
					} else if(q1p2.dot(m_vec)>0.0f && q1q2.dot(m_vec)>0.0f){
						//pEdgeの両端点がthisのendよりm_vecと同じ側にある時
						return true;
					}
				}
			} else{
				//平行でない場合、直線の交点を求めてそれらが線分上に存在するかを求める
				//交点の座標は(x,y)=(x1,y1)+m*(x2,y2)=(x3,y3)+n*(x4,y4) (m,nはfloat)となる
				//0<=m,n<=1であれば交点が両線分上に存在することになる
				//方程式を解くと、m*(x2*y4-x4*y2)=y4*(x3-x1)-x4(y3-y1),n*(x2*y4-x4*y2)=y2*(x3-x1)-x2*(y3-y1)となる。
				const float m=(pEdge->m_vec.y*(pEdge->m_position.x-this->m_position.x)-pEdge->m_vec.x*(pEdge->m_position.y-this->m_position.y))/inclination;
				const float n=(this->m_vec.y*(pEdge->m_position.x-this->m_position.x)-this->m_vec.x*(pEdge->m_position.y-this->m_position.y))/inclination;
				return m>=0.0f && m<=1.0f && n>=0.0f && n<=1.0f;
			}
		}
	} else if(kind==Type::e_polygon){
		//Polygon側に処理を任せる
		return pShape->JudgeCross(this);
	}
	return false;
}

bool Edge::VJudgePointInsideShape(Vector2D point)const{
	//pointへのEdge上の最近傍点を求める
	float dot=m_vec.dot(point-GetBeginPoint());//垂線の位置はm_position+Norm()*dot/Size()となる
	Vector2D nearestPoint;//円の中心に最も近い線分上の点
	if(dot<0.0f){
		//線分外に垂線の足があり、最近傍点がm_positionになる
		nearestPoint=GetBeginPoint();
	} else if(dot<=m_vec.sqSize()){
		//線分内に垂線の足がある場合。
		nearestPoint=m_position+m_vec*dot/m_vec.sqSize();
	} else{
		//線分外に垂線の足があり、最近傍点がm_position+m_vecになる
		nearestPoint=GetEndPoint();
	}
	//最近傍点への距離が5px以内なら点が線上にあるとみなす
	const float capacity=5.0f;
	return (point-nearestPoint).sqSize()<=capacity*capacity;
}

Vector2D Edge::GetLeftTop()const{
	Vector2D ret=m_position;
	if(m_vec.x<0.0f){
		ret.x+=m_vec.x;
	}
	if(m_vec.y<0.0f){
		ret.y+=m_vec.y;
	}
	return ret;
}

Vector2D Edge::GetRightBottom()const{
	Vector2D ret=m_position;
	if(m_vec.x>0.0f){
		ret.x+=m_vec.x;
	}
	if(m_vec.y>0.0f){
		ret.y+=m_vec.y;
	}
	return ret;
}

void Edge::RecordLatticePointInShape(std::vector<int> &latticeInShape,const size_t xNum,const size_t yNum,const size_t squareWidth,const size_t squareHeight,int index)const{
	//線分内は格子点処理を行わない（行えない）ので、このまま放置
}

Vector2D Edge::VGetNearEndpoint(Vector2D point,float capacity)const{
	const float sqCapacity=capacity*capacity;
	if((m_position-point).sqSize()<sqCapacity){
		//始点に近い場合
		return m_position;
	}
	if((m_position+m_vec-point).sqSize()<sqCapacity){
		//終点に近い場合
		return m_position+m_vec;
	}
	//端点に近くないならpointを返す
	return point;
}

void Edge::Resize(Vector2D v){
	//ベクトルをvにする
	m_vec=v;
}

Vector2D Edge::GetRetResize()const{
	//Resize()の逆関数
	return m_vec;
}

void Edge::WriteOutShape(std::ofstream &ofs)const{
	//"("→(種別)→(始点位置)→(初期固定)→(方向ベクトル)→")"
	ofs<<beginer<<Type::GetStr(m_type)<<spliter;//ofs<<"(Edge,";
	ofs<<beginer<<m_position.x<<spliter<<m_position.y<<ender<<spliter;//ofs<<"("<<m_position.x<<","<<m_position.y<<"),";
	ofs<<Fix::GetStr(m_fix)<<spliter;//ofs<<Fix::GetStr(m_fix)<<",";
	ofs<<beginer<<m_vec.x<<spliter<<m_vec.y<<ender<<ender;//ofs<<"("<<m_vec.x<<","<<m_vec.y<<"))";	
}
