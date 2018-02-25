#include"Edge.h"
#include"Circle.h"
#include"DxLib.h"
#include<cassert>

//--------------------Edge--------------------
Edge::Edge(Vector2D begin,Vector2D vec,Fix::Kind fix)
	:Shape(begin,Type::e_edge,fix),m_vec(vec){}

Edge::~Edge(){}

Vector2D Edge::CalculateParpendicularVec(Vector2D point)const{
	//pointからこの直線に垂線を下ろした時、直線からpointに向けた方向の垂線ベクトルを求める
	float dot=m_vec.dot(point-GetBeginPoint());//垂線の足の位置はm_position+Norm()*dotとなる
	Vector2D parpendicularPoint=m_position+m_vec.norm()*dot;//垂線の足の位置
	return parpendicularPoint-point;
}

void Edge::Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	Vector2D begin=m_position+adjust;
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
