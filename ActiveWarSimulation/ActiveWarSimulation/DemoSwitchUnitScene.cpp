#include"DemoSwitchUnitScene.h"
#include"ComputerMoveScene.h"

//------------DemoSwitchUnitScene------------
DemoSwitchUnitScene::DemoSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:SwitchUnitInitOrderScene(battleSceneData){}

DemoSwitchUnitScene::~DemoSwitchUnitScene(){}

int DemoSwitchUnitScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_move):
		//�v���C���[�L��������ȊO�Ȃ�AI����N���X��
		m_nextScene=std::shared_ptr<BattleSceneElement>(new ComputerMoveScene(m_battleSceneData));
		return index;
	case(SceneKind::e_clear):
		if(m_judgeEnd==JudgeEnd::e_playerWin){
			//�v���C���[�̏���
			int deathUnitNum=0;
			for(size_t i=0,size=m_battleSceneData->m_field.size();i<size;i++){
				if(m_battleSceneData->m_field[i]->GetType()==BattleObject::Type::e_unit){
					const Unit *punit=dynamic_cast<const Unit *>(m_battleSceneData->m_field[i]);
					if(!(punit->GetFix()==Shape::Fix::e_static
						|| punit->GetFix()==Shape::Fix::e_dynamic
						)
						)
					{
						//�}�b�v��ɑ��݂��Ă��郆�j�b�g�͐��ɓ���Ȃ�
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
			//m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,true,"�P�ސ��F"+std::to_string(deathUnitNum)+"�l"));//�f���ł͌��ʕ\���͂��Ȃ�
		} else if(m_judgeEnd==JudgeEnd::e_playerLose){
			//�v���C���[�̔s�k
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
							//�}�b�v��ɑ��݂��Ă��郆�j�b�g�͐��ɓ���Ȃ�
							killUnitNum++;
						}
						break;
					default:
						break;
					}
				}
			}
			//m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,false,"���j���F"+std::to_string(killUnitNum)+"�l / "+std::to_string(totalUnitNum)+"�l"));//�f���ł͌��ʕ\���͂��Ȃ�
		}
		return SceneKind::e_gameReset;
	default:
		return index;
	}
}

