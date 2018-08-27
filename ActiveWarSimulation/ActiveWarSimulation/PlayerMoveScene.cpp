#include"DxLib.h"
#include"PlayerMoveScene.h"
#include"GraphicControl.h"

//----------------------PlayerMoveScene------------------------
PlayerMoveScene::PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData)
	,m_waitButton(1520,980,80,80,LoadGraphEX("Graphic/nextButton.png"))
	,m_researchButton(1620,980,80,80,LoadGraphEX("Graphic/researchButton.png"))
	,m_mousePosJustBefore(GetMousePointVector2D())
	,m_mouseLeftFlag(false)
{}

Vector2D PlayerMoveScene::CalculateInputVec()const{
	//移動処理
	Vector2D moveVec;
	if(mouse_get(MOUSE_INPUT_LEFT)>0 && m_mouseLeftFlag){
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
	} else if(analogjoypad_get(DX_INPUT_PAD1)!=Vector2D()){
		//アナログスティック入力があればそれを用いる
		moveVec=analogjoypad_get(DX_INPUT_PAD1);
		//アナログスティックの物理的なズレ等によるmoveVecの微入力を除く
		const float gap=50.0f;
		if(std::abs(moveVec.x)<gap){
			moveVec.x=0.0f;
		}
		if(std::abs(moveVec.y)<gap){
			moveVec.y=0.0f;
		}
	} else{
		//キーボード入力しているなら、それを用いる
		moveVec=Vector2D();
		if(keyboard_get(KEY_INPUT_UP)>0){
			moveVec+=Vector2D(0.0f,-100.0f);
		}
		if(keyboard_get(KEY_INPUT_LEFT)>0){
			moveVec+=Vector2D(-100.0f,0.0f);
		}
		if(keyboard_get(KEY_INPUT_RIGHT)>0){
			moveVec+=Vector2D(100.0f,0.0f);
		}
		if(keyboard_get(KEY_INPUT_DOWN)>0){
			moveVec+=Vector2D(0.0f,100.0f);
		}
	}
	return moveVec;
}

int PlayerMoveScene::thisCalculate(){
	//デバッグコマンドの入力
	if(keyboard_get(KEY_INPUT_LSHIFT)==60){
		m_battleSceneData->m_drawObjectShapeFlag=!m_battleSceneData->m_drawObjectShapeFlag;
	}
	
	//キー入力受付
	const Vector2D mousePos=GetMousePointVector2D();
	if(JudgeAttackCommandUsable()
		&& (keyboard_get(KEY_INPUT_Z)==1
			|| (m_aimedUnit->GetUnitCircleShape()->VJudgePointInsideShape(mousePos) && mouse_get(MOUSE_INPUT_LEFT)==1)
			)
		)
	{
		//攻撃
		//攻撃対象が存在し、OPが足りている場合のみ攻撃処理を行う
		//FinishUnitOperation();//行動終了処理(あとで)
		return SceneKind::e_attackNormal;//攻撃場面へ
	} else if(keyboard_get(KEY_INPUT_D)==1){
		//必殺技

	} else if(keyboard_get(KEY_INPUT_A)==1 && JudgeAttackCommandUsable()){
		//攻撃コマンド使用可能の時のみ、狙いのキャラの変更(反時計回り)
		SetAimedUnit(-1);
	} else if(keyboard_get(KEY_INPUT_S)==1 && JudgeAttackCommandUsable()){
		//攻撃コマンド使用可能の時のみ、狙いのキャラの変更(時計回り)
		SetAimedUnit(1);
	} else if((m_mousePosJustBefore-mousePos).sqSize()>=1.0f
		&& JudgeAttackCommandUsable()
		&& JudgeBecomeAimedUnit(m_battleSceneData->GetUnitPointer(mousePos))
		)
	{
		//攻撃コマンド使用可能の時にマウスを大きく動かしたときのみ、狙いのキャラの変更
		m_aimedUnit=m_battleSceneData->GetUnitPointer(mousePos);
	} else if(keyboard_get(KEY_INPUT_C)==1){
		//アイテムの使用

	} else if(keyboard_get(KEY_INPUT_V)==1 || m_waitButton.JudgePressMoment()){
		//待機
		FinishUnitOperation();
		return 0;
	} else if(keyboard_get(KEY_INPUT_X)==1
		|| keyboard_get(KEY_INPUT_X)>30
		|| mouse_get(MOUSE_INPUT_RIGHT)==1
		|| mouse_get(MOUSE_INPUT_RIGHT)>30
		)
	{
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
	} else if(keyboard_get(KEY_INPUT_F)==1 || m_researchButton.JudgePressMoment()){
		//マップ調べモードへ
		return SceneKind::e_research;
	} else{
		//移動し始めの判定更新(左クリックを押した瞬間であるかを判定・記録する)
		int flame=mouse_get(MOUSE_INPUT_LEFT);
		if(flame==0){
			//左クリックをしなくなると、押していないのでfalseにする
			m_mouseLeftFlag=false;
		} else if(flame==1){
			//1フレーム目を検知できた場合は、trueにする
			m_mouseLeftFlag=true;
		}
		//移動処理
		if(PositionUpdate(CalculateInputVec())){
			//キー入力がなければm_operateUnitの位置更新
			//位置更新をした時の処理
		}
	}

	//次フレームに、本フレームにおけるマウスの位置が分かるようにする
	m_mousePosJustBefore=mousePos;
	
	return SceneKind::e_move;
}

void PlayerMoveScene::thisDraw()const{
	//MoveSceneのをそのまま
	MoveScene::thisDraw();

	//ボタンを描画
	m_waitButton.DrawButton();
	m_researchButton.DrawButton();
}

void PlayerMoveScene::ReturnProcess(){
	//MoveSceneのReturnProcessに加え、m_mouseLeftFlagを初期化する
	MoveScene::ReturnProcess();
	m_mouseLeftFlag=false;
}
