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

int PlayerMoveScene::thisCalculate(){
	//デバッグコマンドの入力
	if(keyboard_get(KEY_INPUT_LSHIFT)==60){
		m_battleSceneData->m_drawObjectShapeFlag=!m_battleSceneData->m_drawObjectShapeFlag;
	}

	//m_operateUnitの位置更新
	if(PositionUpdate(CalculateInputVec())){
		//位置更新をした時の処理

	} else{
		//移動操作をしなかった時はその他の入力を受け付ける
		if(keyboard_get(KEY_INPUT_Z)==1){
			//攻撃
			if(JudgeAttackCommandUsable()){
				//攻撃対象が存在し、OPが足りている場合のみ攻撃処理を行う
				//FinishUnitOperation();//行動終了処理(あとで)
				return SceneKind::e_attackNormal;//攻撃場面へ
			}
		} else if(keyboard_get(KEY_INPUT_D)==1){
			//必殺技

		} else if(keyboard_get(KEY_INPUT_A)==1 && JudgeAttackCommandUsable()){
			//攻撃コマンド使用可能の時のみ、狙いのキャラの変更(反時計回り)
			SetAimedUnit(-1);
		} else if(keyboard_get(KEY_INPUT_S)==1 && JudgeAttackCommandUsable()){
			//攻撃コマンド使用可能の時のみ、狙いのキャラの変更(時計回り)
			SetAimedUnit(1);
		} else if(keyboard_get(KEY_INPUT_C)==1){
			//アイテムの使用

		} else if(keyboard_get(KEY_INPUT_V)==1){
			//待機
			FinishUnitOperation();
			return 0;
		} else if(keyboard_get(KEY_INPUT_X)==1 || keyboard_get(KEY_INPUT_X)>30){
			//移動やり直し(m_route.back()の1つ前の場所に戻す。back()の位置は現在位置の可能性が高いため)
			if(!m_route.empty()){
				m_route.pop_back();
				if(!m_route.empty()){
					//もう要素がなければpop_back()をしない
					RouteInfo info=m_route.back();
					m_route.pop_back();
					//ユニットを移動させる
					m_battleSceneData->m_operateUnit->Warp(info.pos);
					//OPを回復させる
					//m_battleSceneData->m_operateUnit->AddOP(info.OP-m_battleSceneData->m_operateUnit->GetBattleStatus().OP);
					m_battleSceneData->m_operateUnit->SetOP(info.OP);
				}
				//位置更新を行う
				PositionUpdate(Vector2D());
			}
		} else if(keyboard_get(KEY_INPUT_F)==1){
			//マップ調べモードへ
			return SceneKind::e_research;
		}
	}
	return SceneKind::e_move;
}
