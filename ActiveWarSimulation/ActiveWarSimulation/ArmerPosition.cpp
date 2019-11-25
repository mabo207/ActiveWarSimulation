#include"DxLib.h"
#include"ArmerPosition.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"WaitLog.h"

int ArmerPosition::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
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
	int evaluate;
	if(pUnitLogDataList==nullptr){
		//�����ɗ��邱�Ƃ͂Ȃ��͂������A�ꉞ�G���[����
		evaluate=-1;
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
			evaluate=2;
		} else if(attackableEnemyMageCount==1){
			evaluate=1;
		} else{
			evaluate=0;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> ArmerPosition::GetRubricStringInfo(int rubric)const{
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
		rubricStr="Good!!";
		edgeColor=GetColor(196,196,64);
		break;
	}
	return std::make_pair(rubricStr,edgeColor);
}

std::string ArmerPosition::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//�d���������Ȃ�
		comment="�d�������g���Ă݂悤�I";
		break;
	case(0):
		//Bad�]��������
		comment="�d�����𓮂������́A�G�̖����m�ɋ߂Â������Ȃ��悤�ɋC��t���悤�I";
		break;
	case(1):
		//Not Good�]��������
		comment="�G�̖����m���d�����ɋ߂Â��Ă�����A�D��I�ɓ|���Ă݂�̂��ǂ���I";
		break;
	case(2):
		//����ȏ�̕]��
		comment="�������ƂȂ��ł��I���@�g�����d�����ɍU������̂��ł������h���Ă��܂��I";
		break;
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

std::string ArmerPosition::GetReason(int rubric)const{
	switch(rubric){
	case(-1):
		//�`����s��Ȃ�
		return "";
	case(0):
		return "�߂��ɖ����m���������Ċ�Ȃ��ʒu���I";
	case(1):
		return "���̎�ԂŎ������U���ł������ȓG�����m������݂������c�c";
	case(2):
		return "�����m�͍U���ł���ʒu�ɂ͂��Ȃ��݂����I";
	}
	return "";
}
