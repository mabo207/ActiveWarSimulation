#include"ShapeHaving.h"
#include"DxLib.h"

//----------------ShapeHaving-----------------
const float ShapeHaving::pushRate=0.4f;

ShapeHaving::ShapeHaving(std::shared_ptr<Shape> pShape):m_hitJudgeShape(pShape){}

ShapeHaving::~ShapeHaving(){}

void ShapeHaving::ShapeDraw(unsigned int color,int fillFlag,Vector2D adjust)const{
	m_hitJudgeShape->Draw(adjust,color,fillFlag);
}

void ShapeHaving::PosDraw(unsigned int color,int fillFlag,float size,Vector2D adjust)const{
	Vector2D pos=adjust+m_hitJudgeShape->GetPosition();
	DrawCircleAA(pos.x,pos.y,size,color,fillFlag);
}

void ShapeHaving::Move(Vector2D v){
	m_hitJudgeShape->Move(v);
}

void ShapeHaving::Warp(Vector2D v){
	m_hitJudgeShape->Move(v-m_hitJudgeShape->GetPosition());
}

void ShapeHaving::Resize(Vector2D v){
	//基準点からマウスを結んだベクトルを引数に渡す
	m_hitJudgeShape.get()->Resize(v-m_hitJudgeShape->GetPosition());
}

void ShapeHaving::ChangeShape(std::shared_ptr<Shape> pShape){
	m_hitJudgeShape=pShape;
}

bool ShapeHaving::JudgePointInsideShape(Vector2D point)const{
	return m_hitJudgeShape.get()->VJudgePointInsideShape(point);
}

float ShapeHaving::getTop()const {
	return m_hitJudgeShape->GetLeftTop().y;
}

float ShapeHaving::getBottom()const {
	return m_hitJudgeShape->GetRightBottom().y;
}

float ShapeHaving::getLeft()const {
	return m_hitJudgeShape->GetLeftTop().x;
}

float ShapeHaving::getRight()const {
	return m_hitJudgeShape->GetRightBottom().x;
}

Vector2D ShapeHaving::getPos()const {
	return m_hitJudgeShape->GetPosition();
}

const Shape *ShapeHaving::GetHitJudgeShape()const{
	return m_hitJudgeShape.get();
}

Shape::Fix::Kind ShapeHaving::GetFix()const{
	return m_hitJudgeShape->m_fix;
}

Shape::Fix::Kind ShapeHaving::SetFix(Shape::Fix::Kind fix)const{
	return (m_hitJudgeShape->m_fix=fix);
}

Vector2D ShapeHaving::getResizeVec()const{
	//m_hitJudgeShapeのものはオブジェクトの位置を考慮していないのでそれを足して返す
	return m_hitJudgeShape.get()->GetRetResize()+m_hitJudgeShape->GetPosition();
}

const std::vector<const ShapeHaving *> ShapeHaving::InShapeHavingList(const ShapeHaving * const * const pShapeHavingVec,const size_t vecSize)const{
	std::vector<const ShapeHaving *> list;
	const Vector2D v=Vector2D();
	for(size_t i=0;i<vecSize;i++){
		if(m_hitJudgeShape->CalculatePushVec(pShapeHavingVec[i]->GetHitJudgeShape())!=v){
			list.push_back(pShapeHavingVec[i]);
		}
	}
	return list;
}

void ShapeHaving::UpdatePosition(ShapeHaving * const * const pShapeHavingVec,const size_t vecSize,const size_t updateTimes){
	if(m_hitJudgeShape->m_fix==Shape::Fix::e_dynamic){
		//m_fixによって動くかどうかを決める
		//この関数が呼ばれた際図形が存在するのは仮移動先である。仮移動先において、自分と重なっている図形を探し、位置をずらす。
		//重なっている図形を全てリストアップする
		std::vector<const ShapeHaving *> inShapeList;
		for(size_t i=0;i<vecSize;i++){
			//当たり判定に影響を与える図形以外に対して押し出し処理を行う
			if(pShapeHavingVec[i]->GetFix()!=Shape::Fix::e_ignore){
				//図形をぴったり収める長方形を利用して簡易的な当たり判定を行う
				if(JudgeInShapeRect(pShapeHavingVec[i])){
					//押し出しする図形に追加
					inShapeList.push_back(pShapeHavingVec[i]);
				}
			}
		}
		//簡易的当たり判定によって判定できる、重なっている図形の外に出るように押し出し処理をする
		for(size_t i=0;i<updateTimes;i++){
			for(const ShapeHaving *pShapeHaving:inShapeList){
				//各図形に対して完全に押し出せる距離を求める
				Vector2D pushVec=m_hitJudgeShape->CalculatePushVec(pShapeHaving->GetHitJudgeShape());
				//押し出し距離の一定の割合のベクトルだけ押し出す
				this->Move(pushVec*ShapeHaving::pushRate);
			}
		}
	}
}

bool ShapeHaving::JudgeInShapeRect(const ShapeHaving *pShapeHaving)const{
	return this->getLeft()<=pShapeHaving->getRight() && this->getRight()>=pShapeHaving->getLeft() && this->getTop()<=pShapeHaving->getBottom() && this->getBottom()>=pShapeHaving->getTop();
}

