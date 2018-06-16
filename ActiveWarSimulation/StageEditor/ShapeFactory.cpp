#include"DxLib.h"
#include"ShapeFactory.h"
#include"EditActionSettings.h"
#include"BattleObject.h"

#include"input.h"

//--------------------------ShapeFactory::ShapeFactoryButton--------------------------
ShapeFactory::ShapeFactoryButton::ShapeFactoryButton(Vector2D point,Vector2D vec)
	:m_point(point),m_vec(vec){}

bool ShapeFactory::ShapeFactoryButton::JudgeInButton(Vector2D point)const{
	Vector2D v1=point-m_point;
	Vector2D v2=v1-m_vec;
	return(v1.x*v2.x<=0 && v1.y*v2.y<=0);
}

bool ShapeFactory::ShapeFactoryButton::JudgeButtonPushed()const{
	return (mouse_get(MOUSE_INPUT_LEFT)==1);
}

//--------------------------ShapeFactory--------------------------
ShapeFactory::ShapeFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor)
	:m_buttonPos(buttonPos),m_buttonSize(buttonSize),m_buttonLightColor(lightcolor){}

ShapeFactory::~ShapeFactory(){}

void ShapeFactory::DrawPushedButtonLight()const{
	Vector2D v=m_buttonPos+m_buttonSize;
	DrawBox((int)m_buttonPos.x,(int)m_buttonPos.y,(int)v.x,(int)v.y,m_buttonLightColor,TRUE);
}

EditPut::PosSetKind ShapeFactory::VPutAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings){
	if(pskind==EditPut::PosSetKind::BASENONEXIST){
		//置く場所を決めている時
		settings.m_pBattleObject->Warp(point);//位置を確定
		return EditPut::PosSetKind::BASEEXIST;//図形の大きさの決定へ
	} else if(pskind==EditPut::PosSetKind::BASEEXIST){
		//置く図形の大きさを決めている時
		Vector2D pos=settings.m_pBattleObject->getPos();
		settings.m_pBattleObject->Resize(point);//大きさを確定
		settings.PutObject(pos);
		return EditPut::PosSetKind::BASENONEXIST;//図形の位置の決定へ
	}
	return EditPut::PosSetKind::NONEDIT;//例外的な処理
}

void ShapeFactory::VPutNotPressAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings)const{
	if(pskind==EditPut::PosSetKind::BASENONEXIST){
		//置く場所を決めている時
		settings.m_pBattleObject.get()->Warp(point);//図形の位置を変える
	} else if(pskind==EditPut::PosSetKind::BASEEXIST){
		//置く図形の大きさを決めている時
		settings.m_pBattleObject->Resize(point);//図形の大きさを変える
	}
}
