#include"DxLib.h"
#include"MyAngledTriangle.h"
#include"AngledTriangleFactory.h"
#include"EditActionSettings.h"
#include"BattleObject.h"
#include"StageEditor.h"

//-----------------------AngledTriangleFactory::AngledTriangleFactoryButton-----------------------
AngledTriangleFactory::AngledTriangleFactoryButton::AngledTriangleFactoryButton(Vector2D point,Vector2D vec)
	:ShapeFactoryButton(point,vec){}

void AngledTriangleFactory::AngledTriangleFactoryButton::ButtonDraw(int font,int fillFlag)const{
	//îºìßñæâªÇµÇƒÉ{É^ÉìÇÃï`âÊ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//ì‡òg(ÇøÇÂÇ¢ìßâﬂÇ≥ÇπÇÈ)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//ògê¸ÇÃï`âÊ
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//äOòg
	//ìñÇΩÇËîªíËê}å`ÇÃï`âÊ
	const int halfdx=20,halfdy=20;//ëÂÇ´Ç≥(ècâ°ÇªÇÍÇºÇÍ1/2ÇµÇΩèÛë‘)
	DrawTriangle((int)(m_point+m_vec/2).x-halfdx,(int)(m_point+m_vec/2).y-halfdy,(int)(m_point+m_vec/2).x+halfdx,(int)(m_point+m_vec/2).y+halfdy,(int)(m_point+m_vec/2).x-halfdx,(int)(m_point+m_vec/2).y+halfdy,GetColor(255,255,255),TRUE);
}

void AngledTriangleFactory::AngledTriangleFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new AngledTriangleFactory(m_point,m_vec,GetColor(255,255,0)));
	//åªç›ëIëÇµÇƒÇ¢ÇÈÉIÉuÉWÉFÉNÉgÇ‡ÅAìñÇΩÇËîªíËê}å`ÇïœçXÇ∑ÇÈ
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject->ChangeShape(settings.m_pShapeFactory->CreateShape(settings.m_pBattleObject->getPos()));
	}
}

//-----------------------AngledTriangleFactory-----------------------
AngledTriangleFactory::AngledTriangleFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor)
	:ShapeFactory(buttonPos,buttonSize,lightcolor){}

AngledTriangleFactory::~AngledTriangleFactory(){}

std::shared_ptr<Shape> AngledTriangleFactory::CreateShape(Vector2D point)const{
	//return std::shared_ptr<Shape>(new MyAngledTriangle((float)StageEditor::baseSize,(float)(-StageEditor::baseSize)));
	return std::shared_ptr<Shape>(nullptr);
}

