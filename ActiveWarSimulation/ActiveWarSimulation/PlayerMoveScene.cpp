#include"input.h"
#include"DxLib.h"
#include"PlayerMoveScene.h"

//----------------------PlayerMoveScene------------------------
PlayerMoveScene::PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData){}

Vector2D PlayerMoveScene::CalculateInputVec()const{
	Vector2D moveVec;
	if(mouse_get(MOUSE_INPUT_LEFT)>0){
		//マウスを左クリックしているならマウス入力に対応させる
		moveVec=GetMousePointVector2D()-m_battleSceneData->m_operateUnit->getPos();
		//アナログスティックの物理的なズレ等によるmoveVecの微入力を除く
		const float gap=4.0f;
		if(std::abs(moveVec.x)<gap){
			moveVec.x=0.0f;
		}
		if(std::abs(moveVec.y)<gap){
			moveVec.y=0.0f;
		}
	} else{
		//通常はアナログスティック入力
		moveVec=analogjoypad_get(DX_INPUT_PAD1);
		//アナログスティックの物理的なズレ等によるmoveVecの微入力を除く
		const float gap=50.0f;
		if(std::abs(moveVec.x)<gap){
			moveVec.x=0.0f;
		}
		if(std::abs(moveVec.y)<gap){
			moveVec.y=0.0f;
		}
	}
	return moveVec;
}


