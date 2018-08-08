#include"DxLib.h"
#include"SwitchUnitScene.h"
#include"PlayerMoveScene.h"
#include"ComputerMoveScene.h"

//------------------SwitchUnitScene-------------------
SwitchUnitScene::SwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_switch),m_battleSceneData(battleSceneData),m_judgeEnd(JudgeEnd::e_notEnd)
{
	ReturnProcess();
}

SwitchUnitScene::~SwitchUnitScene(){}

int SwitchUnitScene::thisCalculate(){
	//タイマー更新
	m_battleSceneData->m_fpsMesuring.Update();
	
	//勝敗判定によって、遷移処理は分岐
	switch(m_judgeEnd){
	case(JudgeEnd::e_notEnd):
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
		return BattleSceneElement::SceneKind::END;
		break;
	case(JudgeEnd::e_playerLose):
		//プレイヤーが敗北
		return BattleSceneElement::SceneKind::END;
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
}

int SwitchUnitScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_move):
		if(m_battleSceneData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
			//プレイヤーキャラなら自操作クラスへ
			m_nextScene=std::shared_ptr<BattleSceneElement>(new PlayerMoveScene(m_battleSceneData));
		} else{
			//それ以外ならAI操作クラスへ
			m_nextScene=std::shared_ptr<BattleSceneElement>(new ComputerMoveScene(m_battleSceneData));
		}
		return index;
	default:
		return index;
	}
}

void SwitchUnitScene::ReturnProcess(){
	m_battleSceneData->m_fpsMesuring.RecordTime();//タイマー初期化
	m_battleSceneData->FinishUnitOperation();//ユニットの切り替え処理
	//勝敗判定
	size_t playerUnitNum=0,enemyUnitNum=0;
	for(size_t i=0,size=m_battleSceneData->m_unitList.size();i<size;i++){
		if(m_battleSceneData->m_unitList[i]->GetFix()==Shape::Fix::e_static || m_battleSceneData->m_unitList[i]->GetFix()==Shape::Fix::e_dynamic){
			//マップ上に存在しているユニット以外は数に入れない
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
	}
	if(playerUnitNum==0){
		//プレイヤーキャラが0なら負け
		m_judgeEnd=JudgeEnd::e_playerLose;
	} else if(enemyUnitNum==0){
		//敵キャラが0なら勝ち
		m_judgeEnd=JudgeEnd::e_playerWin;
	} else{
		//どれでもないなら戦闘続行
		m_judgeEnd=JudgeEnd::e_notEnd;
	}
}
