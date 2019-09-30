#include"DxLib.h"
#include"ArcherAttackDistance.h"
#include"BattleSceneData.h"
#include"AttackLog.h"

int ArcherAttackDistance::RubricEvaluate(const BattleSceneData * const battleData)const{
	//- ��O����
	//	- �U�����Ȃ�(-1)
	//- �]��
	//	0. �����������A�G���j�b�g�U���˒��ȉ�(�G�̎˒��������̎˒���蒷���ꍇ�͖������ŒB��������̂Ƃ���)
	//	1. ���[�g�������A�����j�b�g�U���˒��ȉ�
	//	2. ���[�g�������A�G�̈ړ������ȉ�
	//	3. 2�ȏ�̕]��
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(battleData->m_scoreObserver->GetLatestLog());
	int evaluate;
	if(!attackLog){
		//���O��AttackLog�łȂ��ꍇ�́u�U�������Ȃ������v�Ɣ��f�ł���
		evaluate=-1;
	} else{
		//�����Ɋւ���]��������
		//�������������߂�
		const float directDistance=(attackLog->GetOperateUnitData().pos-attackLog->GetAimedUnitData().pos).size();
		//��e���j�b�g���s�����j�b�g�ւ̃��[�g���������߂�
		const Unit *operatedUnit=attackLog->GetOperateUnitData().punit;
		const Unit *aimedUnit=attackLog->GetAimedUnit();
		const float routeDistance=CalculateRouteDistance(battleData,attackLog->m_unitDataList,operatedUnit,aimedUnit);
		//�]��(���������画�肵�Ă���)
		if(routeDistance>=attackLog->GetAimedUnit()->GetMaxMoveDistance()){
			evaluate=3;
		} else if(routeDistance>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength()){
			evaluate=2;
		} else if(directDistance>=attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()
			|| attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength())
		{
			evaluate=1;
		} else{
			evaluate=0;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> ArcherAttackDistance::GetRubricStringInfo(int rubric)const{
	std::string rubricStr;
	unsigned int edgeColor;
	switch(rubric){
	case(-1):
		rubricStr="";
		edgeColor=GetColor(0,0,0);
		break;
	case(0):
		//����
		rubricStr="Worst";
		edgeColor=GetColor(128,0,196);
		break;
	case(1):
		//����
		rubricStr="Bad";
		edgeColor=GetColor(96,96,196);
		break;
	case(2):
		//����
		rubricStr="Not good";
		edgeColor=GetColor(128,128,196);
		break;
	case(3):
		//����
		rubricStr="Good!!";
		edgeColor=GetColor(196,196,64);
		break;
	}
	return std::make_pair(rubricStr,edgeColor);
}

std::string ArcherAttackDistance::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//�U�����Ă��Ȃ�
		comment="�����Ǝˎ�ōU�����Ă݂悤�I";
		break;
	case(0):
		//�G���j�b�g�̍U���˒�������̍U��
		comment="�G�̎˒��O����U�����鎖���ӎ����Ă݂�Ɨǂ���I";
		break;
	case(1):
		//���[�g�������U���˒��ȉ�
		comment="�|�ōU�����鎞�́A��Q���□���z���ɍU�����Ă݂悤�I";
		break;
	case(2):
		//���[�g�������G�̈ړ������ȉ�
		comment="�G�����̍s���łǂ��܂ōs���邩���l���āA�G�̍U�����󂯂Ȃ��悤�Ȉʒu�ōU�����邱�Ƃ��ӎ����Ă݂悤�I";
		break;
	case(3):
		//����ȏ�̕]��
		comment="�������ƂȂ��ł��I��Q���□�������܂��g���āA���S�ȏꏊ����U���ł��Ă��܂��I";
		break;
	}
	return comment;
}

std::string ArcherAttackDistance::GetWholeLookBackActionEmpty()const{
	return "�����Ǝˎ�ōU�����Ă݂悤�I";
}

std::string ArcherAttackDistance::GetSubmissionExplanation()const{
	return "�n�`�□���L�����̕ǂ𗘗p���āA\n���S�n�т���U�����Ă݂悤�I";
}

bool ArcherAttackDistance::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_archer);
}
