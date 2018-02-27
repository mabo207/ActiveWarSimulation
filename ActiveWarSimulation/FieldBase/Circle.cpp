#include<cassert>
#include"Circle.h"
#include"DxLib.h"

//--------------------Circle--------------------
Circle::Circle(Vector2D position,float r,Fix::Kind fix)
	:Shape(position,Type::e_circle,fix),m_r(r){}

Circle::~Circle(){}

std::shared_ptr<Shape> Circle::VCopy()const{
	return std::shared_ptr<Shape>(new Circle(this->m_position,this->m_r,this->m_fix));
}

void Circle::Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	Vector2D v=m_position+adjust;
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
		//Edgeの方で処理する。方向に注意、逆向きにする。
		ret=-pShape->CalculatePushVec(this);
		break;
	default:
		ret=Vector2D();
		break;
	}
	return ret;
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

void Circle::Resize(Vector2D v){
	//半径m_rをvの長さにする
	m_r=v.size();
}

Vector2D Circle::GetRetResize()const{
	//長さm_rのベクトルを返せばなんでもよい
	return Vector2D(m_r,0);
}
