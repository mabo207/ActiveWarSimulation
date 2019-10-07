#include"DxLib.h"
#include"MageAttackingOpponent.h"
#include"BattleSceneData.h"
#include"AttackLog.h"

int MageAttackingOpponent::RubricEvaluate(const BattleSceneData * const battleData)const{
	//- ��O����
	//	- �U�����Ȃ�(-1)
	//- �]��
	//	0. �G�������m�E�q�����ł���
	//	1. �G�����m�E�ˎ�ł���
	//	2. �G���d�����ł���
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(battleData->m_scoreObserver->GetLatestLog());
	int evaluate;
	if(!attackLog){
		//���O��AttackLog�łȂ��ꍇ�́u�U�������Ȃ������v�Ɣ��f�ł���
		evaluate=-1;
	} else{
		//�U������̕�������āA�]�����s��
		switch(attackLog->GetAimedUnit()->GetBaseStatus().profession){
		case(Unit::Profession::e_healer):
		case(Unit::Profession::e_mage):
			evaluate=0;
			break;
		case(Unit::Profession::e_archer):
		case(Unit::Profession::e_soldier):
			evaluate=1;
			break;
		case(Unit::Profession::e_armer):
			evaluate=2;
			break;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> MageAttackingOpponent::GetRubricStringInfo(int rubric)const{
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
		//�܂��܂�
		rubricStr="OK";
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

std::string MageAttackingOpponent::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//�U�����Ă��Ȃ�
		comment="�����Ɩ����m�ōU�����Ă݂悤�I";
		break;
	case(0):
		//���@�n���j�b�g�ւ̍U��
		comment="���@�U���́A���@���g���G��蕐��Ő키�G�ɂ����������ʓI����I";
		break;
	case(1):
		//�ˎ�E���m�ւ̍U��
		comment="�ł���΁A������g���G�̒��ł����ɖ��@�Ɏア�d�����������Ƒ_���Ă݂悤�I";
		break;
	case(2):
		//�d�����ւ̍U��
		comment="�������ƂȂ��ł��I���@�Ɏア�G��_���čU���ł��Ă��܂��I";
		break;
	}
	return comment;
}

std::string MageAttackingOpponent::GetWholeLookBackActionEmpty()const{
	return "�����Ɩ����m�ōU�����Ă݂悤�I";
}

std::string MageAttackingOpponent::GetSubmissionExplanation()const{
	return "�����m�ł̍U���̍ۂ́A\n���@�Ɏア�G��_������S�����悤�I";
}

bool MageAttackingOpponent::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_mage);
}
