#include"EditUnitParameter.h"
#include"DxLib.h"
#include"EditActionSettings.h"
#include"Unit.h"

//----------------------EditUnitParameter::EditUnitParameterButton-----------------------
EditUnitParameter::EditUnitParameterButton::EditUnitParameterButton(Vector2D point,Vector2D vec)
	:EditActionButton(point,vec,"EditUnit")
{}

void EditUnitParameter::EditUnitParameterButton::PushedProcess(EditActionSettings &settings)const{
	settings.CancelEditing();
	settings.m_pEditAction=std::shared_ptr<EditAction>(new EditUnitParameter(
		(int)m_point.x
		,(int)m_point.y
		,(int)m_vec.x
		,(int)m_vec.y
		,GetColor(255,255,0)));;
	settings.InitEditObject();
}

//----------------------EditUnitParameter-----------------------
EditUnitParameter::EditUnitParameter(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor)
	:EditAction(buttonX,buttonY,buttonDX,buttonDY,pushedColor)
{}

EditUnitParameter::~EditUnitParameter(){}

void EditUnitParameter::VNonPressEditing(Vector2D,EditActionSettings &settings)const{
	//特に何もしない
}

void EditUnitParameter::VProcessAction(Vector2D point,EditActionSettings &settings){
	if(VGetPosSetKind(settings)==NONEDIT){
		//編集対象が決まっていない場合、pointの地点にあるUnitを探す
		settings.SetEditObject(point);
		if(settings.m_pBattleObject.get()!=nullptr && settings.m_pBattleObject->GetType()!=BattleObject::Type::e_unit){
			//非ユニットは編集対象にしない
			settings.m_pBattleObject=std::shared_ptr<BattleObject>(nullptr);
		}
	} else{
		//編集対象が決まっている場合、編集が終わったものとする
		settings.m_pBattleObject=std::shared_ptr<BattleObject>(nullptr);
	}
}

EditAction::PosSetKind EditUnitParameter::VGetPosSetKind(const EditActionSettings &settings)const{
	if(settings.m_pBattleObject){
		//編集対象が決まっている場合
		return BASEEXIST;
	} else{
		//編集対象が決まっていない場合
		return NONEDIT;
	}
}
