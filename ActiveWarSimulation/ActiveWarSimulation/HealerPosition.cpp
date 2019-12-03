#include"DxLib.h"
#include"HealerPosition.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"WaitLog.h"

SubmissionEvaluation HealerPosition::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- ��O����
	//	- �i�Ȃ��j
	//- �]��
	//	1.�G��������2�̈ȏ�A���샆�j�b�g�����̍s���ōU���\�ł���
	//	2.�G�d������1�̈ȏ�A���샆�j�b�g�����̍s���ōU���\�ł���
	//	3.�G���m��1�̈ȏ�A���샆�j�b�g�����̍s���ōU���\�ł���
	//	4.�G�ˎ肪1�̈ȏ�A���샆�j�b�g�����̍s���ōU���\�ł���
	//	5.�ǂ̓G���������A���샆�j�b�g�����̍s���ōU���ł��Ȃ�
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(evaluateLog);
	const std::vector<LogElement::UnitLogData> *pUnitLogDataList=nullptr;
	std::function<LogElement::UnitLogData()> getAimedUnitData;
	if(attackLog){
		pUnitLogDataList=&attackLog->m_unitDataList;
		getAimedUnitData=[attackLog](){return attackLog->GetOperateUnitData();};
	} else if(waitLog){
		pUnitLogDataList=&waitLog->m_unitDataList;
		getAimedUnitData=[waitLog](){return waitLog->GetOperateUnitData();};
	}
	SubmissionEvaluation evaluate;
	if(pUnitLogDataList==nullptr){
		//�����ɗ��邱�Ƃ͂Ȃ��͂������A�ꉞ�G���[����
		evaluate=SubmissionEvaluation::e_noevaluation;
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
				if(JudgeAttackable(field,stageSize,*pUnitLogDataList,unitData,getAimedUnitData())){
					addFunc();
				}
			}
		}
		//�U���\�ȓG���j�b�g�̐��ŕ]��
		if(attackableEnemyArcherCount+attackableEnemyArmerCount+attackableEnemySoldierCount>=2){
			evaluate=SubmissionEvaluation::e_bad;
		} else if(attackableEnemyArmerCount==1){
			evaluate=SubmissionEvaluation::e_ok;
		} else if(attackableEnemySoldierCount==1){
			evaluate=SubmissionEvaluation::e_good;
		} else if(attackableEnemyArcherCount==1){
			evaluate=SubmissionEvaluation::e_great;
		} else{
			evaluate=SubmissionEvaluation::e_excellent;
		}
	}

	return evaluate;
}

std::string HealerPosition::GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const{
	std::string comment;
	if(mostFrequentEvaluate==SubmissionEvaluation::e_noevaluation){
		//�q�����������Ȃ�
		comment="�q�������g���Ă݂悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_bad){
		//Bad�]��������
		comment="�q�����𓮂������́A�G�ɍU������Ȃ��悤�ȏꏊ�ɓ��������Ƃ��ӎ����悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_ok){
		//Not Good�]��������
		comment="�d�����͍U���͂������̂ŁA�q�������߂Â��������Ȃ��悤�ɋC��t���悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_good){
		//OK�]��������
		comment="���m�͈ړ��͂������̂ŁA�ǖ��̃L������O�ɒu���ē����ǂ��悤�ɓ������̂��ӎ�����Ǝ��₷�����I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_great){
		//Good�]��������
		comment="�ˎ�͍U���͈͂��L���A���̊Ԃɑ̗͂�����Ă��܂��̂ŁA�ł�����葁�߂Ɍ��j���邱�Ƃ�S�����悤�B";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_excellent){
		//Excellent�]��������
		comment="�������ƂȂ��ł��I�q���������S�ȏꏊ�ɔz�u���邱�Ƃ��ӎ��ł��Ă��܂��I";
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

std::string HealerPosition::GetReason(SubmissionEvaluation rubric)const{
	if(rubric==SubmissionEvaluation::e_noevaluation){
		//�`����s��Ȃ�
		return "";
	} else if(rubric==SubmissionEvaluation::e_bad){
		return "�����̓G���U�����Ă��������A��Ȃ��I";
	} else if(rubric==SubmissionEvaluation::e_ok){
		return "�U���͂̍����d�����ɑ_���Ă��܂��ʒu���B";
	} else if(rubric==SubmissionEvaluation::e_good){
		return "�@���͂̂��镺�m�ɖڂ������Ă��܂����悤���c�c";
	} else if(rubric==SubmissionEvaluation::e_great){
		return "�ˎ�ɑ_���Ă���ʒu������A���f�͂ł��Ȃ��������B";
	} else if(rubric==SubmissionEvaluation::e_excellent){
		return "�G���_���Â炢�ʒu�Ɉړ��ł��Ă���ˁI";
	}
	return "";
}