#include"DxLib.h"
#include"SwitchUnitScene.h"
#include"PlayerMoveScene.h"
#include"ComputerMoveScene.h"
#include"StageClearScene.h"
#include"GraphicControl.h"

#include"TutorialPlayerMoveScene.h"

//------------------SwitchUnitScene-------------------
SwitchUnitScene::SwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_switch)
	,m_battleSceneData(battleSceneData)
	,m_judgeEnd(JudgeEnd::e_notEnd)
	,m_rubricWordPosition(-50,-50)
	,m_turnFont(CreateFontToHandleEX("メイリオ",80,8,DX_FONTTYPE_ANTIALIASING_EDGE_4X4,-1,3))
{
	//進行データを更新
	ReturnProcess();
	if(m_judgeEnd==JudgeEnd::e_notEnd){
		//バトル開始直後はかならずe_notEndからe_notEnd_TurnChangeに切り替える
		//しかし、プログラムの構造を変えてターン切り替えのたびにこのコンストラクタが呼ばれるようになると問題になってしまうので、実装を直したい。
		m_judgeEnd=JudgeEnd::e_notEnd_TurnChange;
	}
}

SwitchUnitScene::~SwitchUnitScene(){
	DeleteFontToHandleEX(m_turnFont);
}

int SwitchUnitScene::CalculateTurn()const{
	return m_battleSceneData->CalculateTurn();
}

std::shared_ptr<BattleSceneElement> SwitchUnitScene::GetPlayerMoveScene()const{
	if(m_battleSceneData->m_playMode!=BattleSceneData::PlayMode::e_tutorial){
		//通常バトルである場合はPlayerMoveSceneへ
		return std::shared_ptr<BattleSceneElement>(new PlayerMoveScene(m_battleSceneData));
	} else{
		//チュートリアルである場合はTutorialPlayerMoveSceneへ
		return std::shared_ptr<BattleSceneElement>(new TutorialPlayerMoveScene(m_battleSceneData));
	}
}

int SwitchUnitScene::thisCalculate(){
	//タイマー更新
	m_battleSceneData->m_fpsMesuring.Update();
	//ルーブリック描画位置の更新
	m_rubricWordPosition.Update();
	
	//勝敗判定によって、遷移処理は分岐
	switch(m_judgeEnd){
	case(JudgeEnd::e_notEnd):
	case(JudgeEnd::e_notEnd_TurnChange):
		//勝負がついていない
		//一定時間経ったらユニット移動へ
		if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0f){
			//ユニット移行処理へ
			m_battleSceneData->m_fpsMesuring.RecordTime();//タイマー初期化
			return SceneKind::e_move;
		}
		break;
	case(JudgeEnd::e_playerWin):
		//プレイヤーが勝利
		return SceneKind::e_clear;
		break;
	case(JudgeEnd::e_playerLose):
		//プレイヤーが敗北
		return SceneKind::e_clear;
		break;
	}
	
	return SceneKind::e_switch;
}

void SwitchUnitScene::thisDraw()const{
	//マップの表示
	m_battleSceneData->DrawField();
	//ユニットの表示
	m_battleSceneData->DrawUnit(false,{});
	//HPゲージの描画
	m_battleSceneData->DrawHPGage();
	//オーダーの表示は描画しない

	//ターン切り替え表示
	if(m_judgeEnd==JudgeEnd::e_notEnd_TurnChange){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		DrawBox(1020,750,1920,850,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(mode,pal);
		DrawStringToHandle(1030,760,("BATTLE PHASE "+to_string_0d(CalculateTurn(),2)).c_str(),GetColor(255,255,255),m_turnFont,GetColor(0,0,0));
	}

	//サブミッション評価表示
	if(m_battleSceneData->m_submissionRunFlag){
		m_battleSceneData->m_scoreObserver->GetSubmission().DrawRubric(m_rubricWordPosition.GetX(),m_rubricWordPosition.GetY());
	}
}

int SwitchUnitScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_move):
		if(m_battleSceneData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
			//プレイヤーキャラなら自操作クラスへ
			m_nextScene=GetPlayerMoveScene();
		} else{
			//それ以外ならAI操作クラスへ
			m_nextScene=std::shared_ptr<BattleSceneElement>(new ComputerMoveScene(m_battleSceneData));
		}
		return index;
	case(SceneKind::e_clear):
		m_battleSceneData->m_scoreObserver->FinishUpdate(m_battleSceneData.get());
		if(m_judgeEnd==JudgeEnd::e_playerWin){
			//プレイヤーの勝利
			m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,true));
		} else if(m_judgeEnd==JudgeEnd::e_playerLose){
			//プレイヤーの敗北
			m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,false));
		}
		return index;
	default:
		return index;
	}
}

void SwitchUnitScene::ReturnProcess(){
	m_battleSceneData->m_fpsMesuring.RecordTime();//タイマー初期化
	//ユニット切り替え前にルーブリック評価描画の移動先を設定
	if(m_battleSceneData->m_operateUnit!=nullptr){
		//初期位置の設定
		m_rubricWordPosition=PositionControl((int)m_battleSceneData->m_operateUnit->getPos().x,(int)m_battleSceneData->m_operateUnit->getPos().y-10,10,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
	}
	m_rubricWordPosition.SetTarget(m_rubricWordPosition.GetX(),m_rubricWordPosition.GetY()-50,true);//初期位置より50px上に移動
	//勝敗判定
	size_t playerUnitNum=0,enemyUnitNum=0;
	for(size_t i=0,size=m_battleSceneData->m_unitList.size();i<size;i++){
		switch(m_battleSceneData->m_unitList[i]->GetBattleStatus().team){
		case(Unit::Team::e_player):
			playerUnitNum++;
			break;
		case(Unit::Team::e_enemy):
			enemyUnitNum++;
			break;
		default:
			break;
		}
	}
	if(playerUnitNum==0){
		//プレイヤーキャラが0なら負け
		m_judgeEnd=JudgeEnd::e_playerLose;
	} else if(enemyUnitNum==0){
		//敵キャラが0なら勝ち
		m_judgeEnd=JudgeEnd::e_playerWin;
	} else{
		//どれでもないなら戦闘続行
		const int beforeTurn=CalculateTurn();
		m_battleSceneData->FinishUnitOperation();//ユニットの切り替え処理(戦闘続行以外の時はやらない、クリアターン数の計算が余分に加算されかねないため)
		if(CalculateTurn()!=beforeTurn){
			//切り替え処理の前後でターン数が変わっていたら、ターン切り替え画面を表示するようにする
			m_judgeEnd=JudgeEnd::e_notEnd_TurnChange;
		} else{
			//切り替え処理の前後でターン数が変わっていたら、ターン切り替え表示はしない
			m_judgeEnd=JudgeEnd::e_notEnd;
		}
	}
}
