#include"ShapeHaving.h"
#include"DxLib.h"

//----------------ShapeHaving-----------------
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

std::shared_ptr<Shape> ShapeHaving::getHitJudgeShape()const {
	return m_hitJudgeShape;
}

Vector2D ShapeHaving::getResizeVec()const{
	//m_hitJudgeShapeのものはオブジェクトの位置を考慮していないのでそれを足して返す
	return m_hitJudgeShape.get()->GetRetResize()+m_hitJudgeShape->GetPosition();
}


