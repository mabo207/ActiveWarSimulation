#include"DxLib.h"
#include"EditRemove.h"
#include"EditActionSettings.h"

//---------------------EditRemove::EditRemoveButton---------------------
EditRemove::EditRemoveButton::EditRemoveButton(Vector2D point,Vector2D vec)
	:EditActionButton(point,vec,"remove"){}

void EditRemove::EditRemoveButton::PushedProcess(EditActionSettings &settings)const{
	settings.CancelEditing();
	settings.m_pEditAction=std::shared_ptr<EditAction>(new EditRemove(
		(int)m_point.x
		,(int)m_point.y
		,(int)m_vec.x
		,(int)m_vec.y
		,GetColor(255,255,0)));;
	settings.InitEditObject();
}



//---------------------EditRemove---------------------
EditRemove::EditRemove(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor)
	:EditAction(buttonX,buttonY,buttonDX,buttonDY,pushedColor){}

void EditRemove::VNonPressEditing(Vector2D point,EditActionSettings &settings)const{
	//‰½‚à‚µ‚È‚¢
}

void EditRemove::VProcessAction(Vector2D point,EditActionSettings &settings)const{
	settings.RemoveObject(point);
}

EditAction::PosSetKind EditRemove::VGetPosSetKind(const EditActionSettings &settings)const{
	return NONEDIT;
}
