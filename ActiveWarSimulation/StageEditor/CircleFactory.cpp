#include"DxLib.h"
#include"CircleFactory.h"
#include"Circle.h"
#include"EditActionSettings.h"
#include"BattleObject.h"
#include"StageEditor.h"

//-----------------------CircleFactory::CircleFactoryButton-----------------------
CircleFactory::CircleFactoryButton::CircleFactoryButton(Vector2D point,Vector2D vec)
	:ShapeFactoryButton(point,vec){}

void CircleFactory::CircleFactoryButton::ButtonDraw(int font,int fillFlag)const{
	//半透明化してボタンの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//内枠(ちょい透過させる)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//枠線の描画
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//外枠
	//当たり判定図形の描画
	const float r=20.0f;//半径
	DrawCircleAA((m_point+m_vec/2).x,(m_point+m_vec/2).y,r,30,GetColor(255,255,255),TRUE);
}

void CircleFactory::CircleFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new CircleFactory(m_point,m_vec,GetColor(255,255,0)));
	//現在選択しているオブジェクトも、当たり判定図形を変更する
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject->ChangeShape(settings.m_pShapeFactory->CreateShape(settings.m_pBattleObject->getPos()));
	}
}

//-----------------------CircleFactory-----------------------
CircleFactory::CircleFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor)
	:ShapeFactory(buttonPos,buttonSize,lightcolor){}

CircleFactory::~CircleFactory(){}

std::shared_ptr<Shape> CircleFactory::CreateShape(Vector2D point)const{
	//return std::shared_ptr<Shape>(new MyCircle((float)StageEditor::baseSize));
	return std::shared_ptr<Shape>(new Circle(point,(float)StageEditor::baseSize,Shape::Fix::e_static));
}

