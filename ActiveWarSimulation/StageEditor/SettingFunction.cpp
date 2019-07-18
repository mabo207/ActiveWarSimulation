#include"SettingFunction.h"
#include"DxLib.h"
#include"EditActionSettings.h"
#include"input.h"

//-------------SettingFunction::SettingFunctionButton------------
SettingFunction::SettingFunctionButton::SettingFunctionButton(Vector2D buttonPos,Vector2D buttonVec,const std::string &buttonName,const std::function<void(EditActionSettings &)> &pushedCallback)
	:m_buttonPos(buttonPos)
	,m_buttonVec(buttonVec)
	,m_buttonName(buttonName)
	,m_pushedCallback(pushedCallback)
{}

void SettingFunction::SettingFunctionButton::ButtonDraw(int font,int fillFlag)const{
	const Vector2D end=m_buttonPos+m_buttonVec;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_buttonPos.x,(int)m_buttonPos.y,(int)end.x,(int)end.y,GetColor(192,192,192),fillFlag);//ì‡òg(ÇøÇÂÇ¢ìßâﬂÇ≥ÇπÇÈ)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	DrawBox((int)m_buttonPos.x,(int)m_buttonPos.y,(int)end.x,(int)end.y,GetColor(192,192,64),FALSE);//äOòg
	//ï∂éö
	DrawStringCenterBaseToHandle((int)(m_buttonPos+m_buttonVec/2).x,(int)(m_buttonPos+m_buttonVec/2).y,m_buttonName.c_str(),GetColor(255,255,255),font,true);
}

bool SettingFunction::SettingFunctionButton::JudgeInButton(Vector2D point)const{
	const Vector2D v=point-m_buttonPos;
	return (v.x>=0.0f && v.x<=m_buttonVec.x && v.y>=0.0f && v.y<=m_buttonVec.y);
}

bool SettingFunction::SettingFunctionButton::JudgeButtonPushed()const{
	return mouse_get(MOUSE_INPUT_LEFT)==1;
}

void SettingFunction::SettingFunctionButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pSettingFunction=std::shared_ptr<SettingFunction>(new SettingFunction(m_buttonPos,m_buttonVec,m_pushedCallback,settings));
}

//-------------SettingFunction-------------
SettingFunction::SettingFunction(Vector2D lightUpPos,Vector2D lightUpVec,const std::function<void(EditActionSettings &)> &pushedCallback,EditActionSettings &settings)
	:m_lightUpPos(lightUpPos)
	,m_lightUpVec(lightUpVec)
{
	//âüÇ≥ÇÍÇΩéûÇÃèàóùÇçsÇ§
	pushedCallback(settings);
}

void SettingFunction::LightUpButton()const{
	const Vector2D v=m_lightUpPos+m_lightUpVec;
	DrawBox((int)m_lightUpPos.x,(int)m_lightUpPos.y,(int)v.x,(int)v.y,GetColor(255,255,0),TRUE);
}
