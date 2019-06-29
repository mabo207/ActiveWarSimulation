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
	//半透明化してボタンの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//内枠(ちょい透過させる)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//枠線の描画
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//外枠
	//兵種の描画
	DrawStringCenterBaseToHandle((int)(m_point+m_vec/2).x,(int)(m_point+m_vec/2).y,Unit::Profession::GetName(m_profession).c_str(),GetColor(0,0,0),font,true);
}

void UnitFactory::UnitFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new UnitFactory(m_point,m_vec,GetColor(0,255,255),m_profession));
	//オブジェクトを選択している場合は更新する
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject=settings.m_pShapeFactory->CreateObject(settings.m_pBattleObject->getPos());
	}
}

//----------------UnitFactory-----------------
UnitFactory::UnitFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightColor,const Unit::Profession::Kind profession)
	:ShapeFactory(buttonPos,buttonSize,lightColor)
	,m_profession(profession)
{}

UnitFactory::~UnitFactory(){}

std::shared_ptr<BattleObject> UnitFactory::CreateObject(Vector2D point)const{
	//ひとまずプレイヤー側・Lv1・突撃AI・基本装備のユニットを作成する
	return std::shared_ptr<BattleObject>(Unit::CreateMobUnit("mob",m_profession,1,point,Unit::Team::e_player,Unit::AIType::e_assult,0,{}));
}
