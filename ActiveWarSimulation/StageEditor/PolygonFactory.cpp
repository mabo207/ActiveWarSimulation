#include"DxLib.h"
#include"PolygonFactory.h"
#include"MyPolygon.h"
#include"EditActionSettings.h"
#include"BattleObject.h"
#include"StageEditor.h"

//-----------------------PolygonFactory::PolygonFactoryButton-----------------------
PolygonFactory::PolygonFactoryButton::PolygonFactoryButton(Vector2D point,Vector2D vec)
	:ShapeFactoryButton(point,vec){}

void PolygonFactory::PolygonFactoryButton::ButtonDraw(int font,int fillFlag)const{
	//�����������ă{�^���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//���g(���傢���߂�����)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//�g���̕`��
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//�O�g
	//�����蔻��}�`�̕`��
	const float size=20.0f;
	DrawTriangleAA((m_point+m_vec/2).x-size,(m_point+m_vec/2).y-size,(m_point+m_vec/2).x+size,(m_point+m_vec/2).y+size,(m_point+m_vec/2).x+size-5.0f,(m_point+m_vec/2).y+5.0f,GetColor(255,255,255),TRUE);
	DrawTriangleAA((m_point+m_vec/2).x-size,(m_point+m_vec/2).y-size,(m_point+m_vec/2).x+size,(m_point+m_vec/2).y+size,(m_point+m_vec/2).x+5.0f,(m_point+m_vec/2).y+size-5.0f,GetColor(255,255,255),TRUE);
}

void PolygonFactory::PolygonFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new PolygonFactory(m_point,m_vec,GetColor(255,255,0)));
	//���ݑI�����Ă���I�u�W�F�N�g���A�����蔻��}�`��ύX����
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject->ChangeShape(settings.m_pShapeFactory->CreateShape(settings.m_pBattleObject->getPos()));
	}
}

//-----------------------PolygonFactory-----------------------
PolygonFactory::PolygonFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor)
	:ShapeFactory(buttonPos,buttonSize,lightcolor){}

PolygonFactory::~PolygonFactory(){}

std::shared_ptr<Shape> PolygonFactory::CreateShape(Vector2D point)const{
	//return std::shared_ptr<Shape>(new MyPolygon((float)StageEditor::baseSize));
	return std::shared_ptr<Shape>(new MyPolygon(point,std::vector<Vector2D>{},Shape::Fix::e_static));
}

EditPut::PosSetKind PolygonFactory::VPutAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings)const{
	if(pskind==EditPut::PosSetKind::BASENONEXIST){
		//�u���ꏊ�����߂Ă��鎞
		settings.m_pBattleObject->Warp(point);//�ʒu���m��
		return EditPut::PosSetKind::BASEEXIST;//�}�`�̑傫���̌����
	} else if(pskind==EditPut::PosSetKind::BASEEXIST){
		//�u���}�`�̑傫�������߂Ă��鎞
		Vector2D pos=settings.m_pBattleObject->getPos();
		settings.m_pBattleObject->Resize(point);//�傫�����m��
		settings.PutObject(pos);
		return EditPut::PosSetKind::BASENONEXIST;//�}�`�̈ʒu�̌����
	}
	return EditPut::PosSetKind::NONEDIT;//��O�I�ȏ���

}

void PolygonFactory::VPutNotPressAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings)const{
	if(pskind==EditPut::PosSetKind::BASENONEXIST){
		//�u���ꏊ�����߂Ă��鎞
		settings.m_pBattleObject.get()->Warp(point);//�}�`�̈ʒu��ς���
	} else if(pskind==EditPut::PosSetKind::BASEEXIST){
		//�u���}�`�̑傫�������߂Ă��鎞
		settings.m_pBattleObject->Resize(point);//�}�`�̑傫����ς���
	}
}
