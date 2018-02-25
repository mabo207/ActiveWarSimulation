#include"Shape.h"

//---------------------Shape---------------------
const float Shape::pushRate=0.4f;

Shape::Shape(Vector2D position,Type::Kind type,Fix::Kind fix)
	:m_position(position),m_type(type),m_fix(fix){}

Shape::~Shape(){}

void Shape::Move(Vector2D displacement){
	m_position+=displacement;
}

const std::vector<std::shared_ptr<const Shape>> Shape::InShapeList(const std::vector<std::shared_ptr<const Shape>> &pShapeVec)const{
	std::vector<std::shared_ptr<const Shape>> list;
	const Vector2D v=Vector2D();
	for(const std::shared_ptr<const Shape> &pShape:pShapeVec){
		if(CalculatePushVec(pShape.get())!=v){
			list.push_back(pShape);
		}
	}
	return list;
}

void Shape::Update(const std::vector<std::shared_ptr<Shape>> &pShapeVec,size_t updateTimes){
	if(m_fix==Fix::e_dynamic){
		//m_fixによって動くかどうかを決める
		//この関数が呼ばれた際図形が存在するのは仮移動先である。仮移動先において、自分と重なっている図形を探し、位置をずらす。
		//まず重なっている図形を全てリストアップする
		std::vector<std::shared_ptr<const Shape>> inShapeList;
		for(const std::shared_ptr<const Shape> &pShape:pShapeVec){
			//当たり判定に影響を与える図形以外に対して押し出し処理を行う
			if(pShape->m_fix!=Fix::e_ignore){
				//図形をぴったり収める長方形を利用して簡易的な当たり判定を行う
				if(JudgeInShapeRect(pShape.get())){
					//押し出しする図形に追加
					inShapeList.push_back(pShape);
				}
			}
		}
		//簡易的当たり判定によって判定できる、重なっている図形の外に出るように押し出し処理をする
		for(size_t i=0;i<updateTimes;i++){
			for(const std::shared_ptr<const Shape> &pShape:inShapeList){
				//各図形に対して完全に押し出せる距離を求める
				Vector2D pushVec=CalculatePushVec(pShape.get());
				//押し出し距離の一定の割合のベクトルだけ押し出す
				m_position+=pushVec*Shape::pushRate;
			}
		}
	}
}

bool Shape::JudgeInShapeRect(const Shape *pShape)const{
	return this->GetLeftTop().x<=pShape->GetRightBottom().x && this->GetRightBottom().x>=pShape->GetLeftTop().x && this->GetLeftTop().y<=pShape->GetRightBottom().y && this->GetRightBottom().y>=pShape->GetLeftTop().y;
}
