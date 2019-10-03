#include"DxLib.h"
#include"HealerPosition.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"WaitLog.h"

int HealerPosition::RubricEvaluate(const BattleSceneData * const battleData)const{
	//- ��O����
	//	- �i�Ȃ��j
	//- �]��
	//	1.�G��������2�̈ȏ�A���샆�j�b�g�����̍s���ōU���\�ł���
	//	2.�G�d������1�̈ȏ�A���샆�j�b�g�����̍s���ōU���\�ł���
	//	3.�G���m��1�̈ȏ�A���샆�j�b�g�����̍s���ōU���\�ł���
	//	4.�G�ˎ肪1�̈ȏ�A���샆�j�b�g�����̍s���ōU���\�ł���
	//	5.�ǂ̓G���������A���샆�j�b�g�����̍s���ōU���ł��Ȃ�
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(battleData->m_scoreObserver->GetLatestLog());
	const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(battleData->m_scoreObserver->GetLatestLog());
	const std::vector<LogElement::UnitLogData> *pUnitLogDataList=nullptr;
	const Unit *aimedUnit=nullptr;
	if(attackLog){
		pUnitLogDataList=&attackLog->m_unitDataList;
		aimedUnit=attackLog->GetOperateUnitData().punit;
	} else if(waitLog){
		pUnitLogDataList=&waitLog->m_unitDataList;
		aimedUnit=waitLog->GetOperateUnitData().punit;
	}
	int evaluate;
	if(pUnitLogDataList==nullptr || aimedUnit==nullptr){
		//�����ɗ��邱�Ƃ͂Ȃ��͂������A�ꉞ�G���[����
		evaluate=-1;
	} else{
		//�S�Ă̓G�����A�^�b�J�[�ɑ΂��āA�U���۔�������A�U���\�Ȗ����m�̐����v�Z
		size_t attackableEnemyArcherCount=0;
		size_t attackableEnemySoldierCount=0;
		size_t attackableEnemyArmerCount=0;
		for(const LogElement::UnitLogData &unitData:*pUnitLogDataList){
			if(unitData.punit->GetBattleStatus().team==Unit::Team::e_enemy){
				//�ǂ̃p�����[�^�ɉ��Z���������邩���`����
				std::function<void(void)> addFunc;
				if(unitData.punit->GetBaseStatus().profession==Unit::Profession::e_archer){
					addFunc=[&attackableEnemyArcherCount](){attackableEnemyArcherCount++;};
				} else if(unitData.punit->GetBaseStatus().profession==Unit::Profession::e_soldier){
					addFunc=[&attackableEnemySoldierCount](){attackableEnemySoldierCount++;};
				} else if(unitData.punit->GetBaseStatus().profession==Unit::Profession::e_armer){
					addFunc=[&attackableEnemyArmerCount](){attackableEnemyArmerCount++;};
				} else{
					//���Z�悪�Ȃ��̂ŁA�U���۔�������Ȃ�
					continue;
				}
				//�U���۔���ƃJ�E���g���Z
				if(JudgeAttackable(battleData,*pUnitLogDataList,unitData.punit,aimedUnit)){
					addFunc();
				}
			}
		}
		//�U���\�ȓG���j�b�g�̐��ŕ]��
		if(attackableEnemyArcherCount+attackableEnemyArmerCount+attackableEnemySoldierCount>=2){
			evaluate=0;
		} else if(attackableEnemyArmerCount==1){
			evaluate=1;
		} else if(attackableEnemySoldierCount==1){
			evaluate=2;
		} else if(attackableEnemyArcherCount==1){
			evaluate=3;
		} else{
			evaluate=4;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> HealerPosition::GetRubricStringInfo(int rubric)const{
	std::string rubricStr;
	unsigned int edgeColor;
	switch(rubric){
	case(-1):
		rubricStr="";
		edgeColor=GetColor(0,0,0);
		break;
	case(0):
		//����
		rubricStr="Bad";
		edgeColor=GetColor(96,96,196);
		break;
	case(1):
		//����
		rubricStr="Not good";
		edgeColor=GetColor(128,128,196);
		break;
	case(2):
		//����
		rubricStr="OK";
		edgeColor=GetColor(128,196,196);
		break;
	case(3):
		//�ǂ�
		rubricStr="Good!!";
		edgeColor=GetColor(196,196,64);
		break;
	case(4):
		//�ƂĂ��ǂ�
		rubricStr="Excellent!!";
		edgeColor=GetColor(196,64,128);
		break;
	}
	return std::make_pair(rubricStr,edgeColor);
}

std::string HealerPosition::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//�q�����������Ȃ�
		comment="�q�������g���Ă݂悤�I";
		break;
	case(0):
		//Bad�]��������
		comment="�q�����𓮂������́A�G�ɍU������Ȃ��悤�ȏꏊ�ɓ��������Ƃ��ӎ����悤�I";
		break;
	case(1):
		//Not Good�]��������
		comment="�d�����͍U���͂������̂ŁA�q�������߂Â��������Ȃ��悤�ɋC��t���悤�I";
		break;
	case(2):
		//OK�]��������
		comment="���m�͈ړ��͂������̂ŁA�ǖ��̃L������O�ɒu���ē����ǂ��悤�ɓ������̂��ӎ�����Ǝ��₷�����I";
		break;
	case(3):
		//Good�]��������
		comment="�ˎ�͍U���͈͂��L���A���̊Ԃɑ̗͂�����Ă��܂��̂ŁA�ł�����葁�߂Ɍ��j���邱�Ƃ�S�����悤�B";
		break;
	case(4):
		//Excellent�]��������
		comment="�������ƂȂ��ł��I�q���������S�ȏꏊ�ɔz�u���邱�Ƃ��ӎ��ł��Ă��܂��I";
		break;
	}
	return comment;
}

std::string HealerPosition::GetWholeLookBackActionEmpty()const{
	return "�����Ɖq�����𓮂����Ă݂悤�I";
}

std::string HealerPosition::GetSubmissionExplanation()const{
	return "�q�����͑ł���ア�̂ŁA\n�G�ɍU������Ȃ��悤�ɋC��t���悤�I";
}

bool HealerPosition::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_healer);
}

