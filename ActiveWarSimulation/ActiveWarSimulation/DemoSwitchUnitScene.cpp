#include"DemoSwitchUnitScene.h"
#include"ComputerMoveScene.h"

//------------DemoSwitchUnitScene------------
DemoSwitchUnitScene::DemoSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:SwitchUnitInitOrderScene(battleSceneData){}

DemoSwitchUnitScene::~DemoSwitchUnitScene(){}

int DemoSwitchUnitScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_move):
		//プレイヤーキャもそれ以外ならAI操作クラスへ
		m_nextScene=std::shared_ptr<BattleSceneElement>(new ComputerMoveScene(m_battleSceneData));
		return index;
	case(SceneKind::e_clear):
		if(m_judgeEnd==JudgeEnd::e_playerWin){
			//プレイヤーの勝利
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
			//m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,true,"撤退数："+std::to_string(deathUnitNum)+"人"));//デモでは結果表示はしない
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
			//m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,false,"撃破数："+std::to_string(killUnitNum)+"人 / "+std::to_string(totalUnitNum)+"人"));//デモでは結果表示はしない
		}
		return SceneKind::e_gameReset;
	default:
		return index;
	}
}

