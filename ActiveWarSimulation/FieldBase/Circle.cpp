#include<cassert>
#include"Circle.h"
#include"Edge.h"
#include"MyPolygon.h"
#include"DxLib.h"

//--------------------Circle--------------------
Circle::Circle(Vector2D position,float r,Fix::Kind fix)
	:Shape(position,Type::e_circle,fix),m_r(r){}

Circle::~Circle(){}

std::shared_ptr<Shape> Circle::VCopy()const{
	return std::shared_ptr<Shape>(new Circle(this->m_position,this->m_r,this->m_fix));
}

void Circle::Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	Vector2D v=point+adjust;
	DrawCircleAA(v.x,v.y,m_r,36,color,fillFlag,lineThickness);
}

Vector2D Circle::CalculatePushVec(const Shape *pShape)const{
	Vector2D ret;
	//pShapeの種類によって処理方法が変わる
	switch(pShape->GetType()){
	case Type::e_circle:
		{
			//pShapeをCircleにキャストする
			const Circle *pcircle=dynamic_cast<const Circle *>(pShape);
			if(pcircle!=nullptr){
				//円と円は中心間の距離によって判定が可能
				Vector2D v=this->m_position-pcircle->m_position;//衝突相手から自分に向かう向きに押し出す
				float pushLength=this->GetR()+pcircle->GetR()-v.size();
				if(pushLength<=0.0f){
					//押し戻す長さが負なら、重なっていない事を返す
					ret=Vector2D();
				} else{
					//押し戻す長さが正なら、vをpushLengthになるまで縮める。
					//円の中心が重なると押し出しベクトルが0になるのは諦める（速度が分かっていればなんとかなるが）。
					ret=v.norm()*pushLength;
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
			//pShapeをCircleにキャストする
			const Edge *pedge=dynamic_cast<const Edge *>(pShape);
			if(pedge!=nullptr){
				//Edgeの方で処理する。方向に注意、逆向きにする。
				ret=-pedge->CalculatePushVec(this);
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

bool Circle::PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const{
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
					ret=this->PushParentObj(&e,parentObj,pushRate) | ret;//線分による押し出し処理+当たったかどうかの更新処理
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

bool Circle::JudgeInShape(const Shape *pShape)const{
	//線分の場合は押し出し距離を求められるのでそれを用いる
	return CalculatePushVec(pShape)!=Vector2D();
}

bool Circle::VJudgePointInsideShape(Vector2D point)const{
	//中心とpointまでの距離とm_rを調べれば良い
	return (m_position-point).sqSize()<m_r*m_r;
}

Vector2D Circle::GetLeftTop()const{
	return Vector2D(m_position.x-m_r,m_position.y-m_r);
}

Vector2D Circle::GetRightBottom()const{
	return Vector2D(m_position.x+m_r,m_position.y+m_r);
}

Vector2D Circle::VGetNearEndpoint(Vector2D point,float capacity)const{
	//円に端点は無いのでpointをそのまま返す
	return point;
}

void Circle::Resize(Vector2D v){
	//半径m_rをvの長さにする
	m_r=v.size();
}

Vector2D Circle::GetRetResize()const{
	//長さm_rのベクトルを返せばなんでもよい
	return Vector2D(m_r,0);
}

void Circle::WriteOutShape(std::ofstream &ofs)const{
	//"("→(種別)→(始点位置)→(初期固定)→(半径)→")"
	ofs<<beginer<<Type::GetStr(m_type)<<spliter;//ofs<<"(Circle,";
	ofs<<beginer<<m_position.x<<spliter<<m_position.y<<ender<<spliter;//ofs<<"("<<m_position.x<<","<<m_position.y<<"),";
	ofs<<Fix::GetStr(m_fix)<<spliter;//ofs<<Fix::GetStr(m_fix)<<",";
	ofs<<m_r<<ender;//ofs<<m_r<<")";
}
