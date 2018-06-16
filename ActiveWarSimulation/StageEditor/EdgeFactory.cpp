#include"DxLib.h"
#include"EdgeFactory.h"
#include"Edge.h"
#include"EditActionSettings.h"
#include"BattleObject.h"
#include"StageEditor.h"

//-----------------------EdgeFactory::EdgeFactoryButton-----------------------
EdgeFactory::EdgeFactoryButton::EdgeFactoryButton(Vector2D point,Vector2D vec)
	:ShapeFactoryButton(point,vec){}

void EdgeFactory::EdgeFactoryButton::ButtonDraw(int font,int fillFlag)const{
	//�����������ă{�^���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//���g(���傢���߂�����)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//�g���̕`��
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//�O�g
	//�����蔻��}�`�̕`��
	const Vector2D begin=m_point+m_vec/4;//�n�_
	const Vector2D end=m_point+m_vec*3/4;//�I�_
	DrawLineAA(begin.x,begin.y,end.x,end.y,GetColor(255,255,255),TRUE);
}

void EdgeFactory::EdgeFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new EdgeFactory(m_point,m_vec,GetColor(255,255,0)));
	//���ݑI�����Ă���I�u�W�F�N�g���A�����蔻��}�`��ύX����
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject->ChangeShape(settings.m_pShapeFactory->CreateShape(settings.m_pBattleObject->getPos()));
	}
}

//-----------------------EdgeFactory-----------------------
const Vector2D EdgeFactory::baseVec=Vector2D(((float)StageEditor::baseSize)*0.6f,((float)StageEditor::baseSize)*0.8f);

EdgeFactory::EdgeFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor)
	:ShapeFactory(buttonPos,buttonSize,lightcolor){}

EdgeFactory::~EdgeFactory(){}

std::shared_ptr<Shape> EdgeFactory::CreateShape(Vector2D point)const{
	return std::shared_ptr<Shape>(new Edge(point,baseVec,Shape::Fix::e_static));
}
