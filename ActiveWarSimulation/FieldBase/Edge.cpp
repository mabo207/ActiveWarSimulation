#include"Edge.h"
#include"Circle.h"
#include"MyPolygon.h"
#include"DxLib.h"
#include<cassert>

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

void Edge::Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	Vector2D begin=point+adjust;
	Vector2D end=begin+m_vec;
	DrawLineAA(begin.x,begin.y,end.x,end.y,color,lineThickness);
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
			const Edge *pcircle=dynamic_cast<const Edge *>(pShape);
			if(pcircle!=nullptr){
				//この実装では線と線の当たり判定はできない。なお、実装するなら移動前後の位置が分かる必要がある。
				//使わないので実装しない。
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
