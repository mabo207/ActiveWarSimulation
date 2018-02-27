#include"DxLib.h"
#include"EditAction.h"
#include"input.h"

//-----------------EditAction::EditActionButton-----------------
EditAction::EditActionButton::EditActionButton(Vector2D point,Vector2D vec,const std::string &str)
	:m_point(point),m_vec(vec),m_str(str){}

void EditAction::EditActionButton::ButtonDraw(int font,int fillFlag)const{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//“à˜g(‚¿‚å‚¢“§‰ß‚³‚¹‚é)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//ŠO˜g
	//•¶Žš
	DrawStringCenterBaseToHandle((int)(m_point+m_vec/2).x,(int)(m_point+m_vec/2).y,m_str.c_str(),GetColor(255,255,255),font,true);
}

bool EditAction::EditActionButton::JudgeInButton(Vector2D point)const{
	Vector2D v=point-m_point;
	float minx=min(m_vec.x,0),miny=min(m_vec.y,0);
	float maxx=m_vec.x-minx,maxy=m_vec.y-miny;
	return(minx<=v.x && miny<=v.y && v.x<=maxx && v.y<=maxy);
}

bool EditAction::EditActionButton::JudgeButtonPushed()const{
	return (mouse_get(MOUSE_INPUT_LEFT)==1);
}

//-----------------EditAction-----------------
void EditAction::DrawPushedButton()const{
	DrawBox(m_buttonX,m_buttonY,m_buttonX+m_buttonDX,m_buttonY+m_buttonDY,m_pushedColor,TRUE);
}
