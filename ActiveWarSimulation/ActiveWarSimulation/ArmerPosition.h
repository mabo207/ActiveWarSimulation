#ifndef DEF_ARMERPOSITION_H
#define DEF_ARMERPOSITION_H

#include"SubmissionRuleBase.h"

//�d�����̑ҋ@�|�W�V�����ɂ��Ă̕]�����[��
class ArmerPosition:public SubmissionRuleBase{
public:
	ArmerPosition()=default;
	~ArmerPosition()=default;
	SubmissionEvaluation RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const;//�P�̍s���ɂ��ă��[�u���b�N�]��������
	std::string GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const;//�ł������������[�u���b�N�]���ɂ��āA�����I�ɂȃR�����g��Ԃ�
	std::string GetWholeLookBackActionEmpty()const;//���[���ɊY�������s���������Ȃ��������̑����I�]���̕������Ԃ�
	std::string GetSubmissionExplanation()const;//�T�u�~�b�V�����̐�������Ԃ�
	bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const;//�]�����s�����ǂ���
	std::string GetReason(SubmissionEvaluation rubric)const;//���[�u���b�N�]���̗��R�̐�������Ԃ�
};

#endif // !DEF_ARMERPOSITION_H
