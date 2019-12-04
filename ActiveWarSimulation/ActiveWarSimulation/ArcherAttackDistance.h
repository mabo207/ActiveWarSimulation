#ifndef DEF_ARCHERATTACKDISTANCE_H
#define DEF_ARCHERATTACKDISTANCE_H

#include"SubmissionRuleBase.h"
#include<array>

class ArcherAttackDistance:public SubmissionRuleBase{
public:
	ArcherAttackDistance()=default;
	~ArcherAttackDistance()=default;
	SubmissionEvaluation InAdvanceDataEvaluate(const std::shared_ptr<InAdvanceCalculateDataBase> &,const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const;//�P�̍s���ɂ��ă��[�u���b�N�]��������
	std::string GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const;//�ł������������[�u���b�N�]���ɂ��āA�����I�ɂȃR�����g��Ԃ�
	std::string GetWholeLookBackActionEmpty()const;//���[���ɊY�������s���������Ȃ��������̑����I�]���̕������Ԃ�
	std::string GetSubmissionExplanation()const;//�T�u�~�b�V�����̐�������Ԃ�
	bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const;//�]�����s�����ǂ���
	std::string GetReason(SubmissionEvaluation rubric)const;//���[�u���b�N�]���̗��R�̐�������Ԃ�

	static const std::array<SubmissionEvaluation,4> s_evaluate;//�u�]���Ȃ��v�ȊO�Ŏg����]���ꗗ
};

#endif // !DEF_ARCHERATTACKDISTANCE_H
