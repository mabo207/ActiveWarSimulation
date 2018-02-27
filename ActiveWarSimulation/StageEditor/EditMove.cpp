#include"DxLib.h"
#include"EditMove.h"
#include"EditActionSettings.h"
#include"BattleObject.h"

//---------------------EditMove::EditMoveButton---------------------
EditMove::EditMoveButton::EditMoveButton(Vector2D point,Vector2D vec)
	:EditActionButton(point,vec,"move"){}

void EditMove::EditMoveButton::PushedProcess(EditActionSettings &settings)const{
	settings.CancelEditing();
	settings.m_pEditAction=std::shared_ptr<EditAction>(new EditMove(
		(int)m_point.x
		,(int)m_point.y
		,(int)m_vec.x
		,(int)m_vec.y
		,GetColor(255,255,0)));;
	settings.InitEditObject();
}



//---------------------EditMove---------------------
EditMove::EditMove(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor)
	:EditAction(buttonX,buttonY,buttonDX,buttonDY,pushedColor){}

void EditMove::VNonPressEditing(Vector2D point,EditActionSettings &settings)const{
	//当たり判定図形に対してWarpを常に行う
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject.get()->Warp(point);
	}
}

void EditMove::VProcessAction(Vector2D point,EditActionSettings &settings)const{
	//位置変更を行うBattleObjectが決まっているかどうかで行う行為が変わる
	if(VGetPosSetKind(settings)==BASENONEXIST){
		//編集対象が決まっている場合、その位置変更を行う
		settings.m_pBattleObject.get()->Warp(point);
		settings.InitEditObject();
	}else{
		//編集対象が決まっていない場合、pointの地点にあるBattleObjectを探す
		settings.SetEditObject(point);
	}
}

EditAction::PosSetKind EditMove::VGetPosSetKind(const EditActionSettings &settings)const{
	if(settings.m_pBattleObject.get()!=nullptr){
		//編集対象が決まっている場合
		return BASENONEXIST;
	}else{
		//編集対象が決まっていない場合
		return NONEDIT;
	}
}
