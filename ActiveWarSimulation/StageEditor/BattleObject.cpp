#include"DxLib.h"
#include"BattleObject.h"

#include"Terrain.h"

std::string BattleObject::Type::GetStr(Kind kind){
	switch(kind){
	case(e_terrain):
		return "Terrain";
	case(e_unit):
		return "Unit";
	default:
		return "";
	}
}

void BattleObject::VDraw(Vector2D adjust)const{
	VDraw(m_hitJudgeShape->GetPosition(),adjust);
}

void BattleObject::ShapeDraw(unsigned int color,int fillFlag,float lineThickness,Vector2D adjust)const{
	m_hitJudgeShape->Draw(adjust,color,fillFlag,lineThickness);
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

void BattleObject::WriteOutObjectWholeInfo(std::ofstream &ofs)const{
	//"{"��(�I�u�W�F�N�g���L���)��","��(�}�`���)��"}\n"�Əo�͂���
	ofs<<"{";
	WriteOutObjectPeculiarInfo(ofs);
	ofs<<",";
	m_hitJudgeShape->WriteOutShape(ofs);
	ofs<<"}"<<std::endl;
}

std::shared_ptr<BattleObject> BattleObject::CreateObject(const std::string &infostr){
	//str��StringBuilder��p���ĕ�������
	StringBuilder sb(infostr,',','(',')',false,true);
	//str�̉��߁Bsb.m_vec[0]:�I�u�W�F�N�g�̎�� strVec[1]:�����蔻��}�`�̏��
	if(sb.m_vec.size()<2){
		return std::shared_ptr<BattleObject>(nullptr);
	}
	//�����蔻��}�`�̐���
	std::shared_ptr<Shape> pShape=Shape::CreateShape(sb.m_vec[1].GetString());
	//�I�u�W�F�N�g�̐���
	std::shared_ptr<BattleObject> pb(nullptr);
	if(pShape.get()!=nullptr){
		//�����蔻��}�`��nullptr�Ȃ�I�u�W�F�N�g�����͍s�킸��nullptr��Ԃ��B
		const std::string objectname=sb.m_vec[0].GetString();
		if(objectname==Type::GetStr(Type::e_terrain)){
			pb=std::shared_ptr<BattleObject>(new Terrain(pShape,-1,GetColor(128,128,128),true));
		}
	}

	return pb;
}
