#include"DxLib.h"
#include"RectangleFactory.h"
#include"EditActionSettings.h"
#include"MyRectangle.h"
#include"BattleObject.h"
#include"StageEditor.h"

//--------------------------RectangleFactory::RectangleFactoryButton--------------------------
RectangleFactory::RectangleFactoryButton::RectangleFactoryButton(Vector2D point,Vector2D vec)
	:ShapeFactoryButton(point,vec){}

void RectangleFactory::RectangleFactoryButton::ButtonDraw(int font,int fillFlag)const{
	//半透明化してボタンの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//内枠(ちょい透過させる)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//枠線の描画
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//外枠
	//当たり判定図形の描画
	const int halfdx=20,halfdy=20;//大きさ(縦横それぞれ1/2した状態)
	DrawBox((int)(m_point+m_vec/2).x-halfdx,(int)(m_point+m_vec/2).y-halfdy,(int)(m_point+m_vec/2).x+halfdx,(int)(m_point+m_vec/2).y+halfdy,GetColor(255,255,255),TRUE);
}

void RectangleFactory::RectangleFactoryButton::PushedProcess(EditActionSettings &settings)const{
	//pShapeFactoryを変更する
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new RectangleFactory(m_point,m_vec,GetColor(255,255,0)));
	//現在選択しているオブジェクトも、当たり判定図形を変更する
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject->ChangeShape(settings.m_pShapeFactory->CreateShape(settings.m_pBattleObject->getPos()));
	}
}

//--------------------------RectangleFactory--------------------------
RectangleFactory::RectangleFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor)
	:ShapeFactory(buttonPos,buttonSize,lightcolor){}

RectangleFactory::~RectangleFactory(){}

std::shared_ptr<Shape> RectangleFactory::CreateShape(Vector2D point)const{
	//return std::shared_ptr<Shape>(new MyRectangle((float)StageEditor::baseSize,(float)StageEditor::baseSize));
	return std::shared_ptr<Shape>(nullptr);
}
