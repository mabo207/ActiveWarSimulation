#include"DxLib.h"
#include"BattleObject.h"
#include"GraphicControl.h"
#include"StringBuilder.h"

#include"Terrain.h"

BattleObject::~BattleObject(){
	DeleteGraphEX(m_gHandle);
}

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

void BattleObject::WriteOutObjectWholeInfo(std::ofstream &ofs)const{
	//"{"��(�I�u�W�F�N�g���L���)��","��(�}�`���)��"}\n"�Əo�͂���
	ofs<<"{";
	WriteOutObjectPeculiarInfo(ofs);
	ofs<<",";
	m_hitJudgeShape->WriteOutShape(ofs);
	ofs<<"}"<<std::endl;
}

std::shared_ptr<BattleObject> BattleObject::CreateObject(const std::string &infostr){
	//CreateRawObject()��p����
	return std::shared_ptr<BattleObject>(CreateRawObject(infostr));
}

BattleObject *BattleObject::CreateRawObject(const std::string &infostr){
	//str��StringBuilder��p���ĕ�������
	StringBuilderOld sb(infostr,',','(',')',false,true);
	//str�̉��߁Bsb.m_vec[0]:�I�u�W�F�N�g�̎�� strVec[1]:�����蔻��}�`�̏��
	if(sb.m_vec.size()<2){
		return nullptr;
	}
	//�����蔻��}�`�̐���
	std::shared_ptr<Shape> pShape=Shape::CreateShape(sb.m_vec[1].GetString());
	//�I�u�W�F�N�g�̐���
	BattleObject *pb(nullptr);
	if(pShape.get()!=nullptr){
		//�����蔻��}�`��nullptr�Ȃ�I�u�W�F�N�g�����͍s�킸��nullptr��Ԃ��B
		const std::string objectname=sb.m_vec[0].GetString();
		if(objectname==Type::GetStr(Type::e_terrain)){
			pb=new Terrain(pShape,-1,GetColor(128,128,128),true);
		}
	}

	return pb;
}
