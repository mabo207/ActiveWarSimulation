#include"DxLib.h"
#include"UnitFactory.h"
#include"EditActionSettings.h"
#include"StageEditor.h"
#include"Circle.h"

//----------------UnitFactory::UnitFactoryButton-------------------
UnitFactory::UnitFactoryButton::UnitFactoryButton(Vector2D point,Vector2D vec,Unit::Profession::Kind profession)
	:ShapeFactoryButton(point,vec)
	,m_profession(profession)
{}

void UnitFactory::UnitFactoryButton::ButtonDraw(int font,int fillFlag)const{
	//îºìßñæâªÇµÇƒÉ{É^ÉìÇÃï`âÊ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//ì‡òg(ÇøÇÂÇ¢ìßâﬂÇ≥ÇπÇÈ)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//ògê¸ÇÃï`âÊ
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//äOòg
	//ï∫éÌÇÃï`âÊ
	DrawStringCenterBaseToHandle((int)(m_point+m_vec/2).x,(int)(m_point+m_vec/2).y,Unit::Profession::GetName(m_profession).c_str(),GetColor(0,0,0),font,true);
}

void UnitFactory::UnitFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new UnitFactory(m_point,m_vec,GetColor(0,255,255),m_profession));
}

//----------------UnitFactory-----------------
UnitFactory::UnitFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightColor,const Unit::Profession::Kind profession)
	:ShapeFactory(buttonPos,buttonSize,lightColor)
	,m_profession(profession)
{}

UnitFactory::~UnitFactory(){}

std::shared_ptr<Shape> UnitFactory::CreateShape(Vector2D point)const{
	return std::shared_ptr<Shape>(new Circle(point,Unit::unitCircleSize,Shape::Fix::e_dynamic));
}
