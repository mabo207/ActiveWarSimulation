#include"DxLib.h"
#include"ArmerPosition.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"WaitLog.h"

SubmissionEvaluation ArmerPosition::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- ��O����
	//	- �i�Ȃ��j
	//- �]��
	//	1. �G�����m��2�̈ȏ�A���샆�j�b�g�����̍s���ōU���\�ł���
	//	2. �G�����m��1�̈ȏ�A���샆�j�b�g�����̍s���ōU���\�ł���
	//	3. �ǂ̓G�����m���A���샆�j�b�g�����̍s���ōU���ł��Ȃ�
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(evaluateLog);
	const std::vector<LogElement::UnitLogData> *pUnitLogDataList=nullptr;
	std::function<LogElement::UnitLogData()> getAimedUnitFunc;
	if(attackLog){
		pUnitLogDataList=&attackLog->m_unitDataList;
		getAimedUnitFunc=[attackLog](){return attackLog->GetOperateUnitData();};
	} else if(waitLog){
		pUnitLogDataList=&waitLog->m_unitDataList;
		getAimedUnitFunc=[waitLog](){return waitLog->GetOperateUnitData();};
	}
	SubmissionEvaluation evaluate;
	if(pUnitLogDataList==nullptr){
		//�����ɗ��邱�Ƃ͂Ȃ��͂������A�ꉞ�G���[����
		evaluate=SubmissionEvaluation::e_noevaluation;
	} else{
		//�S�Ă̓G�����m�ɑ΂��āA�U���۔�������A�U���\�Ȗ����m�̐����v�Z
		size_t attackableEnemyMageCount=0;
		for(const LogElement::UnitLogData &unitData:*pUnitLogDataList){
			if(unitData.punit->GetBaseStatus().profession==Unit::Profession::e_mage && unitData.punit->GetBattleStatus().team==Unit::Team::e_enemy){
				if(JudgeAttackable(field,stageSize,*pUnitLogDataList,unitData,getAimedUnitFunc())){
					attackableEnemyMageCount++;
				}
			}
		}
		//�U���\�Ȗ����m�̐��ŕ]��
		if(attackableEnemyMageCount==0){
			evaluate=SubmissionEvaluation::e_excellent;
		} else if(attackableEnemyMageCount==1){
			evaluate=SubmissionEvaluation::e_ok;
		} else{
			evaluate=SubmissionEvaluation::e_bad;
		}
	}

	return evaluate;
}

std::string ArmerPosition::GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const{
	std::string comment;
	if(mostFrequentEvaluate==SubmissionEvaluation::e_noevaluation){
		//�d���������Ȃ�
		comment="�d�������g���Ă݂悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_bad){
		//Bad�]��������
		comment="�d�����𓮂������́A�G�̖����m�ɋ߂Â������Ȃ��悤�ɋC��t���悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_ok){
		//Not Good�]��������
		comment="�G�̖����m���d�����ɋ߂Â��Ă�����A�D��I�ɓ|���Ă݂�̂��ǂ���I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_excellent){
		//����ȏ�̕]��
		comment="�������ƂȂ��ł��I���@�g�����d�����ɍU������̂��ł������h���Ă��܂��I";
	}
	return comment;
}

std::string ArmerPosition::GetWholeLookBackActionEmpty()const{
	return "�����Əd�����𓮂����Ă݂悤�I";
}

std::string ArmerPosition::GetSubmissionExplanation()const{
	return "�d�����͖��@�U���Ɏア�̂ŁA\n�����m�ɍU������Ȃ��悤�ɋC��t���悤�I";
}

bool ArmerPosition::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_armer);
}

std::string ArmerPosition::GetReason(SubmissionEvaluation rubric)const{
	if(rubric==SubmissionEvaluation::e_noevaluation){
		//�`����s��Ȃ�
		return "";
	} else if(rubric==SubmissionEvaluation::e_bad){
		return "�߂��ɖ����m���������Ċ�Ȃ��ʒu���I";
	} else if(rubric==SubmissionEvaluation::e_ok){
		return "���̎�ԂŎ������U���ł������ȓG�����m������݂������c�c";
	} else if(rubric==SubmissionEvaluation::e_excellent){
		return "�����m�͍U���ł���ʒu�ɂ͂��Ȃ��݂����I";
	}
	return "";
}
