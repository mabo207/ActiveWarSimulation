#include"DxLib.h"
#include"SwitchUnitScene.h"
#include"PlayerMoveScene.h"
#include"ComputerMoveScene.h"
#include"StageClearSceme.h"

//------------------SwitchUnitScene-------------------
SwitchUnitScene::SwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_switch),m_battleSceneData(battleSceneData),m_judgeEnd(JudgeEnd::e_notEnd)
{
	ReturnProcess();
}

SwitchUnitScene::~SwitchUnitScene(){}

int SwitchUnitScene::thisCalculate(){
	//�^�C�}�[�X�V
	m_battleSceneData->m_fpsMesuring.Update();
	
	//���s����ɂ���āA�J�ڏ����͕���
	switch(m_judgeEnd){
	case(JudgeEnd::e_notEnd):
		//���������Ă��Ȃ�
		//��莞�Ԍo�����烆�j�b�g�ړ���
		if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0f){
			//���j�b�g�ڍs������
			m_battleSceneData->m_fpsMesuring.RecordTime();//�^�C�}�[������
			return SceneKind::e_move;
		}
		break;
	case(JudgeEnd::e_playerWin):
		//�v���C���[������
		return SceneKind::e_clear;
		break;
	case(JudgeEnd::e_playerLose):
		//�v���C���[���s�k
		return SceneKind::e_clear;
		break;
	}
	
	return SceneKind::e_switch;
}

void SwitchUnitScene::thisDraw()const{
	//�}�b�v�̕\��
	m_battleSceneData->DrawField();
	//���j�b�g�̕\��
	m_battleSceneData->DrawUnit(false,{});
	//HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();
	//�I�[�_�[�̕\���͕`�悵�Ȃ�
}

std::shared_ptr<BattleSceneElement> SwitchUnitScene::GetPlayerOperateClass()const{
	return std::shared_ptr<BattleSceneElement>(new PlayerMoveScene(m_battleSceneData));
}

int SwitchUnitScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_move):
		if(m_battleSceneData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
			//�v���C���[�L�����Ȃ玩����N���X��
			m_nextScene=GetPlayerOperateClass();
		} else{
			//����ȊO�Ȃ�AI����N���X��
			m_nextScene=std::shared_ptr<BattleSceneElement>(new ComputerMoveScene(m_battleSceneData));
		}
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
			m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,true,"�P�ސ��F"+std::to_string(deathUnitNum)+"�l"));
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
			m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,false,"���j���F"+std::to_string(killUnitNum)+"�l / "+std::to_string(totalUnitNum)+"�l"));
		}
		return index;
	default:
		return index;
	}
}

void SwitchUnitScene::ReturnProcess(){
	m_battleSceneData->m_fpsMesuring.RecordTime();//�^�C�}�[������
	m_battleSceneData->FinishUnitOperation();//���j�b�g�̐؂�ւ�����
	//���s����
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
		//�v���C���[�L������0�Ȃ畉��
		m_judgeEnd=JudgeEnd::e_playerLose;
	} else if(enemyUnitNum==0){
		//�G�L������0�Ȃ珟��
		m_judgeEnd=JudgeEnd::e_playerWin;
	} else{
		//�ǂ�ł��Ȃ��Ȃ�퓬���s
		m_judgeEnd=JudgeEnd::e_notEnd;
	}
}
