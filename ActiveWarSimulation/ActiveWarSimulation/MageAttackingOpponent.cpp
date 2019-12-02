#include"DxLib.h"
#include"MageAttackingOpponent.h"
#include"BattleSceneData.h"
#include"AttackLog.h"

SubmissionEvaluation MageAttackingOpponent::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- ��O����
	//	- �U�����Ȃ�(-1)
	//- �]��
	//	0. �G�������m�E�q�����ł���
	//	1. �G�����m�E�ˎ�ł���
	//	2. �G���d�����ł���
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	SubmissionEvaluation evaluate;
	if(!attackLog){
		//���O��AttackLog�łȂ��ꍇ�́u�U�������Ȃ������v�Ɣ��f�ł���
		evaluate=SubmissionEvaluation::e_noevaluation;
	} else{
		//�U������̕�������āA�]�����s��
		switch(attackLog->GetAimedUnit()->GetBaseStatus().profession){
		case(Unit::Profession::e_healer):
		case(Unit::Profession::e_mage):
			evaluate=SubmissionEvaluation::e_bad;
			break;
		case(Unit::Profession::e_archer):
		case(Unit::Profession::e_soldier):
			evaluate=SubmissionEvaluation::e_ok;
			break;
		case(Unit::Profession::e_armer):
			evaluate=SubmissionEvaluation::e_excellent;
			break;
		}
	}

	return evaluate;
}

std::string MageAttackingOpponent::GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const{
	std::string comment;
	if(mostFrequentEvaluate==SubmissionEvaluation::e_noevaluation){
		//�U�����Ă��Ȃ�
		comment="�����Ɩ����m�ōU�����Ă݂悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_bad){
		//���@�n���j�b�g�ւ̍U��
		comment="���@�U���́A���@���g���G��蕐��Ő키�G�ɂ����������ʓI����I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_ok){
		//�ˎ�E���m�ւ̍U��
		comment="�ł���΁A������g���G�̒��ł����ɖ��@�Ɏア�d�����������Ƒ_���Ă݂悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_excellent){
		//�d�����ւ̍U��
		comment="�������ƂȂ��ł��I���@�Ɏア�G��_���čU���ł��Ă��܂��I";
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

std::string MageAttackingOpponent::GetReason(SubmissionEvaluation rubric)const{
	if(rubric==SubmissionEvaluation::e_noevaluation){
		//�`����s��Ȃ�
		return "";
	} else if(rubric==SubmissionEvaluation::e_bad){
		return "�����m��q�����ɂ͖��@�U���͌��ʂ������݂������B";
	} else if(rubric==SubmissionEvaluation::e_ok){
		return "���m��ˎ�𖂖@�ōU������̂́A�܂��܂����ʓI���B";
	} else if(rubric==SubmissionEvaluation::e_excellent){
		return "���@����_�̏d�������U���ł��Ă��܂��I";
	}
	return "";
}
