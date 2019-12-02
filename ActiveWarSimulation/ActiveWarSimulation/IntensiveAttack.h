#ifndef DEF_INTENSIVEATTACK_H
#define DEF_INTENSIVEATTACK_H

#include"SubmissionRuleBase.h"

//�W���U���ɂ��Ă̕]�����[��
class IntensiveAttack:public SubmissionRuleBase{
public:
	IntensiveAttack()=default;
	~IntensiveAttack()=default;
	SubmissionEvaluation RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const;//�P�̍s���ɂ��ă��[�u���b�N�]��������
	std::string GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const;//�ł������������[�u���b�N�]���ɂ��āA�����I�ɂȃR�����g��Ԃ�
	std::string GetWholeLookBackActionEmpty()const;//���[���ɊY�������s���������Ȃ��������̑����I�]���̕������Ԃ�
	std::string GetSubmissionExplanation()const;//�T�u�~�b�V�����̐�������Ԃ�
	bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const;//�]�����s�����ǂ���
	std::string GetReason(SubmissionEvaluation rubric)const;//���[�u���b�N�]���̗��R�̐�������Ԃ�
};

#endif // !DEF_INTENSIVEATTACK_H
