#include"DxLib.h"

#include"EditPut.h"
#include"ShapeFactory.h"
#include"EditActionSettings.h"

#include"BattleObject.h"
#include"Terrain.h"

//---------------------EditPut::EditPutButton---------------------
EditPut::EditPutButton::EditPutButton(Vector2D point,Vector2D vec)
	:EditActionButton(point,vec,"put"){}

void EditPut::EditPutButton::PushedProcess(EditActionSettings &settings)const{
	settings.CancelEditing();
	settings.m_pEditAction=std::shared_ptr<EditAction>(new EditPut(
		(int)m_point.x
		,(int)m_point.y
		,(int)m_vec.x
		,(int)m_vec.y
		,GetColor(255,255,0)));
	settings.m_pBattleObject=std::shared_ptr<BattleObject>(new Terrain(settings.m_pShapeFactory.get()->CreateShape(Vector2D(0.0f,0.0f)),-1,GetColor(128,128,128),false));
}

//---------------------EditPut---------------------
EditPut::EditPut(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor)
	:EditAction(buttonX,buttonY,buttonDX,buttonDY,pushedColor),m_setKind(PosSetKind::BASENONEXIST){}

void EditPut::VNonPressEditing(Vector2D point,EditActionSettings &settings)const{
	if(m_setKind==PosSetKind::BASENONEXIST){
		//�u���ꏊ�����߂Ă��鎞
		settings.m_pBattleObject.get()->Warp(point);//�}�`�̈ʒu��ς���
	} else if(m_setKind==PosSetKind::BASEEXIST){
		//�u���}�`�̑傫�������߂Ă��鎞
		settings.m_pBattleObject->Resize(point);//�}�`�̑傫����ς���
	}
}

void EditPut::VProcessAction(Vector2D point,EditActionSettings &settings){
/*
	if(m_setKind==PosSetKind::BASENONEXIST){
		//�u���ꏊ�����߂Ă��鎞
		settings.m_pBattleObject->Warp(point);//�ʒu���m��
		m_setKind=PosSetKind::BASEEXIST;//�}�`�̑傫���̌����
	} else if(m_setKind==PosSetKind::BASEEXIST){
		//�u���}�`�̑傫�������߂Ă��鎞
		Vector2D pos=settings.m_pBattleObject->getPos();
		settings.m_pBattleObject->Resize(point);//�傫�����m��
		settings.PutObject(pos);
		m_setKind=PosSetKind::BASENONEXIST;//�}�`�̈ʒu�̌����
	}
//*/
	if(settings.m_pShapeFactory.get()!=nullptr){
		m_setKind=settings.m_pShapeFactory->VPutAction(m_setKind,point,settings);
	}
}

EditAction::PosSetKind EditPut::VGetPosSetKind(const EditActionSettings &settings)const{
	return m_setKind;
}
