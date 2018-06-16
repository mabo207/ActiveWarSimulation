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
	//半透明化してボタンの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//内枠(ちょい透過させる)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//枠線の描画
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//外枠
	//当たり判定図形の描画
	const Vector2D begin=m_point+m_vec/4;//始点
	const Vector2D end=m_point+m_vec*3/4;//終点
	DrawLineAA(begin.x,begin.y,end.x,end.y,GetColor(255,255,255),TRUE);
}

void EdgeFactory::EdgeFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new EdgeFactory(m_point,m_vec,GetColor(255,255,0)));
	//現在選択しているオブジェクトも、当たり判定図形を変更する
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
