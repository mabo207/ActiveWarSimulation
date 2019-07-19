#include"DxLib.h"
#include"PlayerMoveScene.h"
#include"GraphicControl.h"
#include"GeneralPurposeResource.h"
#include<iostream>
#include"FilePath.h"
#include"CommonConstParameter.h"

namespace{
	int GetSubmissionX(){
		return CommonConstParameter::gameResolutionX-SelfDecideSubmission::s_submissionWidth;
	}
}

//----------------------PlayerMoveScene------------------------
const std::array<std::function<std::pair<bool,int>(PlayerMoveScene&)>,11> PlayerMoveScene::inCalculateProcessFunction={
	&PlayerMoveScene::AttackProcess
	,&PlayerMoveScene::SkillProcess
	,&PlayerMoveScene::CounterclockwiseProcess
	,&PlayerMoveScene::ClockwiseProcess
	,&PlayerMoveScene::MouseSetAimedUnitProcess
	,&PlayerMoveScene::ItemProcess
	,&PlayerMoveScene::WaitProcess
	,&PlayerMoveScene::CancelProcess
	,&PlayerMoveScene::ResearchProcess
	,&PlayerMoveScene::SystemMenuProcess
	,&PlayerMoveScene::MoveProcess
};

std::pair<bool,int> PlayerMoveScene::AttackProcess(){
	const Vector2D mousePos=GetMousePointVector2D();
	if(JudgeAttackCommandUsable()
		&& (keyboard_get(KEY_INPUT_Z)==1
			|| (m_aimedUnit->GetUnitCircleShape()->VJudgePointInsideShape(mousePos)
				//&& mouse_get(MOUSE_INPUT_LEFT)==1
				&& BattleSceneData::JudgeMousePushInsideMapDrawZone(MOUSE_INPUT_LEFT,false)
				)
			)
		&& m_attackableOnlyChangeInherit
		)
	{
		//攻撃
		//攻撃対象が存在し、OPが足りている場合のみ攻撃処理を行う
		//FinishUnitOperation();//行動終了処理(あとで)
		return std::pair<bool,int>(true,SceneKind::e_attackNormal);//攻撃場面へ
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::SkillProcess(){
	if(keyboard_get(KEY_INPUT_D)==1
		)
	{
		//必殺技
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::CounterclockwiseProcess(){
	if(keyboard_get(KEY_INPUT_A)==1
		&& JudgeAttackCommandUsable()
		&& m_attackableOnlyChangeInherit
		)
	{
		//攻撃コマンド使用可能の時のみ、狙いのキャラの変更(反時計回り)
		SetAimedUnit(-1);
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);//選択の効果音再生
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::ClockwiseProcess(){
	if(keyboard_get(KEY_INPUT_S)==1
		&& JudgeAttackCommandUsable()
		&& m_attackableOnlyChangeInherit
		)
	{
		//攻撃コマンド使用可能の時のみ、狙いのキャラの変更(時計回り)
		SetAimedUnit(1);
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);//選択の効果音再生
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::MouseSetAimedUnitProcess(){
	const Vector2D mousePos=GetMousePointVector2D();
	if((m_mousePosJustBefore-mousePos).sqSize()>=1.0f
		&& JudgeAttackCommandUsable()
		&& JudgeBecomeAimedUnit(m_battleSceneData->GetUnitPointer(mousePos))
		&& m_attackableOnlyChangeInherit
		)
	{
		//攻撃コマンド使用可能の時にマウスを大きく動かしたときのみ、狙いのキャラの変更
		const Unit *beforeAimedUnit=m_aimedUnit;
		m_aimedUnit=m_battleSceneData->GetUnitPointer(mousePos);
		if(m_aimedUnit!=nullptr && m_aimedUnit!=beforeAimedUnit){
			//選択ユニットが変更されていれば狙い切り替え音を鳴らす
			PlaySoundMem(m_battleSceneData->m_aimchangeSound,DX_PLAYTYPE_BACK,TRUE);
		}
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::ItemProcess(){
	if(keyboard_get(KEY_INPUT_C)==1){
		//アイテムの使用
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::WaitProcess(){
	if(
		(
			keyboard_get(KEY_INPUT_V)==1
			|| mouse_get(MOUSE_INPUT_MIDDLE)==1
			|| m_waitButton.JudgePressMoment()
			)
		&& m_waitableOnlyChangeInherit
		)
	{
		//待機
		FinishUnitOperation();
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//待機決定は決定音
		return std::pair<bool,int>(true,0);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::CancelProcess(){
	if(
		(
			keyboard_get(KEY_INPUT_X)==1
			|| keyboard_get(KEY_INPUT_X)>30
			|| mouse_get(MOUSE_INPUT_RIGHT)==1
			|| mouse_get(MOUSE_INPUT_RIGHT)>30
			)
		&& m_moveableOnlyChangeInherit
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
		//スコアシステム処理
		m_battleSceneData->m_scoreObserver->CancelUpdate();
		//キャンセル処理をしたので、trueを返す
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::ResearchProcess(){
	if(keyboard_get(KEY_INPUT_F)==1 || m_researchButton.JudgePressMoment()){
		//マップ調べモードへ
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
		return std::pair<bool,int>(true,SceneKind::e_research);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::SystemMenuProcess(){
	if(keyboard_get(KEY_INPUT_SPACE)==1 || m_menuButton.JudgePressMoment()){
		//システムメニュー画面へ
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
		return std::pair<bool,int>(true,SceneKind::e_systemMenu);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::MoveProcess(){
	if(m_moveableOnlyChangeInherit){
		//移動し始めの判定更新(左クリックを押した瞬間であるかを判定・記録する)
		int frame=mouse_get(MOUSE_INPUT_LEFT);
		if(frame==0){
			//左クリックをしなくなると、押していないのでfalseにする
			m_mouseLeftFlag=false;
		} else if(frame==1){
			//1フレーム目を検知できた場合は、trueにする
			m_mouseLeftFlag=true;
		}
		//移動処理
		if(PositionUpdate(CalculateInputVec())){
			//キー入力がなければm_operateUnitの位置更新
			//位置更新をした時の処理
		}
	}
	return std::pair<bool,int>(true,SceneKind::e_move);
}

void PlayerMoveScene::EnableAllChangeInherit(){
	m_moveableOnlyChangeInherit=true;
	m_attackableOnlyChangeInherit=true;
	m_waitableOnlyChangeInherit=true;
}

PlayerMoveScene::PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData)
	,m_waitButton(1520,980,80,80,LoadGraphEX(FilePath::graphicDir+"nextButton.png"))
	,m_researchButton(1620,980,80,80,LoadGraphEX(FilePath::graphicDir+"researchButton.png"))
	,m_menuButton(1820,980,80,80,LoadGraphEX(FilePath::graphicDir+"menuButton.png"))
	,m_mousePosJustBefore(GetMousePointVector2D())
	,m_mouseLeftFlag(false)
	,m_attackableOnlyChangeInherit(true)
	,m_moveableOnlyChangeInherit(true)
	,m_waitableOnlyChangeInherit(true)
	,m_submissionPosition(GetSubmissionX(),-SelfDecideSubmission::s_submissionHeight,10,Easing::TYPE_IN,Easing::FUNCTION_LINER,0.0)
	,m_notOperateFrame(0)
{}

Vector2D PlayerMoveScene::CalculateInputVec()const{
	//移動処理
	Vector2D moveVec;
	if(//mouse_get(MOUSE_INPUT_LEFT)>0
		BattleSceneData::JudgeMousePushInsideMapDrawZone(MOUSE_INPUT_LEFT,true)
		&& m_mouseLeftFlag
		)
	{
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

	std::pair<bool,int> retPal;
	for(const std::function<std::pair<bool,int>(PlayerMoveScene&)> &func:inCalculateProcessFunction){
		retPal=func(*this);
		if(retPal.first && retPal.second!=SceneKind::e_move){
			return retPal.second;
		}
	}

/*
	if(JudgeAttackCommandUsable()
		&& (keyboard_get(KEY_INPUT_Z)==1
			|| (m_aimedUnit->GetUnitCircleShape()->VJudgePointInsideShape(mousePos)
				//&& mouse_get(MOUSE_INPUT_LEFT)==1
				&& BattleSceneData::JudgeMousePushInsideMapDrawZone(MOUSE_INPUT_LEFT,false)
				)
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
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);//選択の効果音再生
	} else if(keyboard_get(KEY_INPUT_S)==1 && JudgeAttackCommandUsable()){
		//攻撃コマンド使用可能の時のみ、狙いのキャラの変更(時計回り)
		SetAimedUnit(1);
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);//選択の効果音再生
	} else if((m_mousePosJustBefore-mousePos).sqSize()>=1.0f
		&& JudgeAttackCommandUsable()
		&& JudgeBecomeAimedUnit(m_battleSceneData->GetUnitPointer(mousePos))
		)
	{
		//攻撃コマンド使用可能の時にマウスを大きく動かしたときのみ、狙いのキャラの変更
		const Unit *beforeAimedUnit=m_aimedUnit;
		m_aimedUnit=m_battleSceneData->GetUnitPointer(mousePos);
		if(m_aimedUnit!=nullptr && m_aimedUnit!=beforeAimedUnit){
			//選択ユニットが変更されていれば狙い切り替え音を鳴らす
			PlaySoundMem(m_battleSceneData->m_aimchangeSound,DX_PLAYTYPE_BACK,TRUE);
		}
	} else if(keyboard_get(KEY_INPUT_C)==1){
		//アイテムの使用

	} else if(keyboard_get(KEY_INPUT_V)==1 || m_waitButton.JudgePressMoment()){
		//待機
		FinishUnitOperation();
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//待機決定は決定音
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
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
		return SceneKind::e_research;
	} else if(keyboard_get(KEY_INPUT_SPACE)==1 || m_menuButton.JudgePressMoment()){
		//システムメニュー画面へ
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
		return SceneKind::e_systemMenu;
	} else{
		//移動し始めの判定更新(左クリックを押した瞬間であるかを判定・記録する)
		int frame=mouse_get(MOUSE_INPUT_LEFT);
		if(frame==0){
			//左クリックをしなくなると、押していないのでfalseにする
			m_mouseLeftFlag=false;
		} else if(frame==1){
			//1フレーム目を検知できた場合は、trueにする
			m_mouseLeftFlag=true;
		}
		//移動処理
		if(PositionUpdate(CalculateInputVec())){
			//キー入力がなければm_operateUnitの位置更新
			//位置更新をした時の処理
		}
	}
//*/

	//無操作時間の更新(マウスのみで)
	if(mousePos!=m_mousePosJustBefore){
		m_notOperateFrame=0;
	} else{
		m_notOperateFrame++;
	}
	printfDx("frame:%d y:%d targety:%d\n",m_notOperateFrame,m_submissionPosition.GetY(),m_submissionPosition.GetendY());

	//サブミッションの位置の更新
	if(m_notOperateFrame==0){
		//操作されたら、画面外に移動させる
		//動かし続けても外に移動しなくなってしまうため、bool値は工夫する
		m_submissionPosition.SetTarget(GetSubmissionX(),-SelfDecideSubmission::s_submissionHeight,m_submissionPosition.GetendY()!=-SelfDecideSubmission::s_submissionHeight);
	} else if(m_notOperateFrame==120){
		//2秒無操作状態が続いたら画面内に移動させる
		m_submissionPosition.SetTarget(GetSubmissionX(),0,true);
	} else{
		m_submissionPosition.Update();
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
	m_menuButton.DrawButton();

	//サブミッション描画
	if(m_battleSceneData->m_submissionRunFlag){
		m_battleSceneData->m_scoreObserver->GetSubmission().DrawSubmission(m_submissionPosition.GetX(),m_submissionPosition.GetY());
	}
}

void PlayerMoveScene::ReturnProcess(){
	//MoveSceneのReturnProcessに加え、m_mouseLeftFlagを初期化する
	MoveScene::ReturnProcess();
	m_mouseLeftFlag=false;
}
