#include"DxLib.h"
#include"CircleFactory.h"
#include"Circle.h"
#include"EditActionSettings.h"
#include"StageEditor.h"
#include"Terrain.h"

//-----------------------CircleFactory::CircleFactoryButton-----------------------
CircleFactory::CircleFactoryButton::CircleFactoryButton(Vector2D point,Vector2D vec)
	:ShapeFactoryButton(point,vec){}

void CircleFactory::CircleFactoryButton::ButtonDraw(int font,int fillFlag)const{
	//�����������ă{�^���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//���g(���傢���߂�����)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//�g���̕`��
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//�O�g
	//�����蔻��}�`�̕`��
	const float r=20.0f;//���a
	DrawCircleAA((m_point+m_vec/2).x,(m_point+m_vec/2).y,r,30,GetColor(255,255,255),TRUE);
}

void CircleFactory::CircleFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new CircleFactory(m_point,m_vec,GetColor(255,255,0)));
	//�I�u�W�F�N�g��I�����Ă���ꍇ�͍X�V����
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject=settings.m_pShapeFactory->CreateObject(settings.m_pBattleObject->getPos());
	}
}

//-----------------------CircleFactory-----------------------
CircleFactory::CircleFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor)
	:ShapeFactory(buttonPos,buttonSize,lightcolor){}

CircleFactory::~CircleFactory(){}

std::shared_ptr<BattleObject> CircleFactory::CreateObject(Vector2D point)const{
	//return std::shared_ptr<Shape>(new MyCircle((float)StageEditor::baseSize));
	const std::shared_ptr<Shape> pShape(new Circle(point,(float)StageEditor::baseSize,Shape::Fix::e_static));
	return std::shared_ptr<BattleObject>(new Terrain(pShape,-1,GetColor(128,128,128),false));
}

