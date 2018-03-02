#include"DxLib.h"
#include"BattleObject.h"

void BattleObject::VDraw(Vector2D adjust)const{
	VDraw(m_hitJudgeShape->GetPosition(),adjust);
}

void BattleObject::ShapeDraw(unsigned int color,int fillFlag,Vector2D adjust)const{
	m_hitJudgeShape->Draw(adjust,color,fillFlag);
}

void BattleObject::PosDraw(unsigned int color,int fillFlag,float size,Vector2D adjust)const{
	Vector2D pos=adjust+m_hitJudgeShape->GetPosition();
	DrawCircleAA(pos.x,pos.y,size,color,fillFlag);
}

void BattleObject::Move(Vector2D v){
	m_hitJudgeShape->Move(v);
}

void BattleObject::Warp(Vector2D v){
	m_hitJudgeShape->Move(v-m_hitJudgeShape->GetPosition());
}

void BattleObject::Resize(Vector2D v){
	//��_����}�E�X�����񂾃x�N�g���������ɓn��
	m_hitJudgeShape.get()->Resize(v-m_hitJudgeShape->GetPosition());
}

void BattleObject::ChangeShape(std::shared_ptr<Shape> pShape){
	m_hitJudgeShape=pShape;
}

bool BattleObject::JudgePointInsideShape(Vector2D point)const{
	return m_hitJudgeShape.get()->VJudgePointInsideShape(point);
}

float BattleObject::getTop()const {
	return m_hitJudgeShape->GetLeftTop().y;
}

float BattleObject::getBottom()const {
	return m_hitJudgeShape->GetRightBottom().y;
}

float BattleObject::getLeft()const {
	return m_hitJudgeShape->GetLeftTop().x;
}

float BattleObject::getRight()const {
	return m_hitJudgeShape->GetRightBottom().x;
}

Vector2D BattleObject::getPos()const {
	return m_hitJudgeShape->GetPosition();
}

std::shared_ptr<Shape> BattleObject::getHitJudgeShape()const {
	return m_hitJudgeShape;
}

Vector2D BattleObject::getResizeVec()const{
	//m_hitJudgeShape�̂��̂̓I�u�W�F�N�g�̈ʒu���l�����Ă��Ȃ��̂ł���𑫂��ĕԂ�
	return m_hitJudgeShape.get()->GetRetResize()+m_hitJudgeShape->GetPosition();
}

