#ifndef DEF_ARCHERATTACKDISTANCE_H
#define DEF_ARCHERATTACKDISTANCE_H

#include"SubmissionRuleBase.h"
#include<array>

class ArcherAttackDistance:public SubmissionRuleBase{
public:
	ArcherAttackDistance()=default;
	~ArcherAttackDistance()=default;
	//���[�u���b�N�]���֌W
	std::shared_ptr<InAdvanceCalculateDataBase> CalculateInAdvanceData(
		const std::vector<BattleObject *> &field
		,const Vector2D stageSize
		,const std::shared_ptr<const LogElement> &evaluateLog)const;
	SubmissionEvaluation InAdvanceDataEvaluate(
		const std::shared_ptr<InAdvanceCalculateDataBase> &inAdvanceData
		,const std::vector<BattleObject *> &field
		,const Vector2D stageSize
		,const std::shared_ptr<const LogElement> &evaluateLog)const;
	//���̑��̊֐�
	std::string GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const;//�ł������������[�u���b�N�]���ɂ��āA�����I�ɂȃR�����g��Ԃ�
	std::string GetWholeLookBackActionEmpty()const;//���[���ɊY�������s���������Ȃ��������̑����I�]���̕������Ԃ�
	std::string GetSubmissionExplanation()const;//�T�u�~�b�V�����̐�������Ԃ�
	bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const;//�]�����s�����ǂ���
	std::string GetReason(SubmissionEvaluation rubric)const;//���[�u���b�N�]���̗��R�̐�������Ԃ�

	static const std::array<SubmissionEvaluation,4> s_evaluate;//�u�]���Ȃ��v�ȊO�Ŏg����]���ꗗ
};

#endif // !DEF_ARCHERATTACKDISTANCE_H
