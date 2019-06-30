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
	//���ɉ������Ȃ�
}

void EditUnitParameter::VProcessAction(Vector2D point,EditActionSettings &settings){
	if(VGetPosSetKind(settings)==NONEDIT){
		//�ҏW�Ώۂ����܂��Ă��Ȃ��ꍇ�Apoint�̒n�_�ɂ���Unit��T��
		settings.SetEditObject(point);
		if(settings.m_pBattleObject.get()!=nullptr && settings.m_pBattleObject->GetType()!=BattleObject::Type::e_unit){
			//�񃆃j�b�g�͕ҏW�Ώۂɂ��Ȃ�
			settings.m_pBattleObject=std::shared_ptr<BattleObject>(nullptr);
		}
	} else{
		//�ҏW�Ώۂ����܂��Ă���ꍇ�A�ҏW���I��������̂Ƃ���
		settings.m_pBattleObject=std::shared_ptr<BattleObject>(nullptr);
	}
}

EditAction::PosSetKind EditUnitParameter::VGetPosSetKind(const EditActionSettings &settings)const{
	if(settings.m_pBattleObject){
		//�ҏW�Ώۂ����܂��Ă���ꍇ
		return BASEEXIST;
	} else{
		//�ҏW�Ώۂ����܂��Ă��Ȃ��ꍇ
		return NONEDIT;
	}
}
