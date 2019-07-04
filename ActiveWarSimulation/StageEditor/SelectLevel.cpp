#include"DxLib.h"
#include"SelectLevel.h"
#include"EditActionSettings.h"
#include"input.h"

//-------------SelectLevel::SelectLevelButton------------
SelectLevel::SelectLevelButton::SelectLevelButton(Vector2D buttonPos,Vector2D buttonVec,StageLevel level)
	:m_buttonPos(buttonPos)
	,m_buttonVec(buttonVec)
	,m_level(level)
{}

void SelectLevel::SelectLevelButton::ButtonDraw(int font,int fillFlag)const{
	const Vector2D end=m_buttonPos+m_buttonVec;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_buttonPos.x,(int)m_buttonPos.y,(int)end.x,(int)end.y,GetColor(192,192,192),fillFlag);//“à˜g(‚¿‚å‚¢“§‰ß‚³‚¹‚é)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	DrawBox((int)m_buttonPos.x,(int)m_buttonPos.y,(int)end.x,(int)end.y,GetColor(192,192,64),FALSE);//ŠO˜g
	//•¶Žš
	DrawStringCenterBaseToHandle((int)(m_buttonPos+m_buttonVec/2).x,(int)(m_buttonPos+m_buttonVec/2).y,m_level.GetString().c_str(),GetColor(255,255,255),font,true);
}

bool SelectLevel::SelectLevelButton::JudgeInButton(Vector2D point)const{
	const Vector2D v=point-m_buttonPos;
	return (v.x>=0.0f && v.x<=m_buttonVec.x && v.y>=0.0f && v.y<=m_buttonVec.y);
}

bool SelectLevel::SelectLevelButton::JudgeButtonPushed()const{
	return mouse_get(MOUSE_INPUT_LEFT)==1;
}

void SelectLevel::SelectLevelButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pSelectLevel=std::shared_ptr<SelectLevel>(new SelectLevel(m_buttonPos,m_buttonVec,m_level));
}

//-------------SelectLevel-------------
SelectLevel::SelectLevel(Vector2D lightUpPos,Vector2D lightUpVec,StageLevel level)
	:m_lightUpPos(lightUpPos)
	,m_lightUpVec(lightUpVec)
	,m_level(level)
{}

void SelectLevel::LightUpButton()const{
	const Vector2D v=m_lightUpPos+m_lightUpVec;
	DrawBox((int)m_lightUpPos.x,(int)m_lightUpPos.y,(int)v.x,(int)v.y,GetColor(255,255,0),TRUE);
}

std::string SelectLevel::GetUnitListFileName()const{
	return "SaveData/unitlist_"+m_level.GetString()+".txt";
}
