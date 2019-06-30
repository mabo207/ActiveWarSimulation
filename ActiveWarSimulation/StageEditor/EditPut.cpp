#include"DxLib.h"

#include"EditPut.h"
#include"ShapeFactory.h"
#include"EditActionSettings.h"

#include"Terrain.h"

//---------------------EditPut::EditPutButton---------------------
EditPut::EditPutButton::EditPutButton(Vector2D point,Vector2D vec)
	:EditActionButton(point,vec,"put"){}

void EditPut::EditPutButton::PushedProcess(EditActionSettings &settings)const{
	settings.CancelEditing();
	settings.m_pEditAction=std::shared_ptr<EditAction>(new EditPut(
		(int)m_point.x
		,(int)m_point.y
		,(int)m_vec.x
		,(int)m_vec.y
		,GetColor(255,255,0)));
	settings.m_pBattleObject=settings.m_pShapeFactory->CreateObject(Vector2D(0.0f,0.0f));
}

//---------------------EditPut---------------------
EditPut::EditPut(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor)
	:EditAction(buttonX,buttonY,buttonDX,buttonDY,pushedColor),m_setKind(PosSetKind::BASENONEXIST){}

void EditPut::VNonPressEditing(Vector2D point,EditActionSettings &settings)const{
	if(settings.m_pShapeFactory.get()!=nullptr){
		settings.m_pShapeFactory->VPutNotPressAction(m_setKind,point,settings);
	}
}

void EditPut::VProcessAction(Vector2D point,EditActionSettings &settings){
	if(settings.m_pShapeFactory.get()!=nullptr){
		m_setKind=settings.m_pShapeFactory->VPutAction(m_setKind,point,settings);
	}
}

EditAction::PosSetKind EditPut::VGetPosSetKind(const EditActionSettings &settings)const{
	return m_setKind;
}

void EditPut::ActionDraw(const Vector2D adjust,const EditActionSettings &settings)const{
	//•\Ž¦•û–@‚ªì¬}Œ`‚É‚æ‚Á‚Ä•Ï‚í‚é
	settings.m_pShapeFactory->FactoryDraw(adjust,settings);
}
