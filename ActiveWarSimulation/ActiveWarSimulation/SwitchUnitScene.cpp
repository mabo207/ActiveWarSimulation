#include"DxLib.h"
#include"SwitchUnitScene.h"
#include"PlayerMoveScene.h"
#include"ComputerMoveScene.h"
#include"StageClearSceme.h"
#include"GraphicControl.h"

#include"TutorialPlayerMoveScene.h"

//------------------SwitchUnitScene-------------------
SwitchUnitScene::SwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_switch)
	,m_battleSceneData(battleSceneData)
	,m_judgeEnd(JudgeEnd::e_notEnd)
	,m_turnFont(CreateFontToHandleEX("メイリオ",80,8,DX_FONTTYPE_ANTIALIASING_EDGE_4X4,-1,3))
{
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
	return (int)(m_battleSceneData->m_totalOP/Unit::BattleStatus::maxOP)+1;
}

int SwitchUnitScene::thisCalculate(){
	//タイマー更新
	m_battleSceneData->m_fpsMesuring.Update();
	
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
}

int SwitchUnitScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_move):
		if(m_battleSceneData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
			//プレイヤーキャラなら自操作クラスへ
			if(m_battleSceneData->m_playMode!=BattleSceneData::PlayMode::e_tutorial){
				//通常バトルである場合はPlayerMoveSceneへ
				m_nextScene=std::shared_ptr<BattleSceneElement>(new PlayerMoveScene(m_battleSceneData));
			} else{
				//チュートリアルである場合はTutorialPlayerMoveSceneへ
				m_nextScene=std::shared_ptr<BattleSceneElement>(new TutorialPlayerMoveScene(m_battleSceneData));
			}
		} else{
			//それ以外ならAI操作クラスへ
			m_nextScene=std::shared_ptr<BattleSceneElement>(new ComputerMoveScene(m_battleSceneData));
		}
		return index;
	case(SceneKind::e_clear):
		if(m_judgeEnd==JudgeEnd::e_playerWin){
			//プレイヤーの勝利
			//撤退数の記録
			int deathUnitNum=0;
			for(size_t i=0,size=m_battleSceneData->m_field.size();i<size;i++){
				if(m_battleSceneData->m_field[i]->GetType()==BattleObject::Type::e_unit){
					const Unit *punit=dynamic_cast<const Unit *>(m_battleSceneData->m_field[i]);
					if(!(punit->GetFix()==Shape::Fix::e_static
						|| punit->GetFix()==Shape::Fix::e_dynamic
						)
						)
					{
						//マップ上に存在しているユニットは数に入れない
						switch(punit->GetBattleStatus().team){
						case(Unit::Team::e_player):
							deathUnitNum++;
							break;
						default:
							break;
						}
					}
				}
			}
			//クリアターン数の記録
			int clearTurn=(int)(m_battleSceneData->m_totalOP/Unit::BattleStatus::maxOP)+1;//0~150.0fが1ターンなので+1する
			//nextSceneの作成
			const std::string str=
				"撤退数："+std::to_string(deathUnitNum)+"人\n"
				+"クリアターン数："+std::to_string(clearTurn);
			m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,true,str));
		} else if(m_judgeEnd==JudgeEnd::e_playerLose){
			//プレイヤーの敗北
			int killUnitNum=0,totalUnitNum=0;
			for(size_t i=0,size=m_battleSceneData->m_field.size();i<size;i++){
				if(m_battleSceneData->m_field[i]->GetType()==BattleObject::Type::e_unit){
					const Unit *punit=dynamic_cast<const Unit *>(m_battleSceneData->m_field[i]);
					switch(punit->GetBattleStatus().team){
					case(Unit::Team::e_enemy):
						totalUnitNum++;
						if(!(punit->GetFix()==Shape::Fix::e_static
							|| punit->GetFix()==Shape::Fix::e_dynamic
							)
							)
						{
							//マップ上に存在しているユニットは数に入れない
							killUnitNum++;
						}
						break;
					default:
						break;
					}
				}
			}
			m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,false,"撃破数："+std::to_string(killUnitNum)+"人 / "+std::to_string(totalUnitNum)+"人"));
		}
		return index;
	default:
		return index;
	}
}

void SwitchUnitScene::ReturnProcess(){
	m_battleSceneData->m_fpsMesuring.RecordTime();//タイマー初期化
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
