#ifndef DEF_PROTECTFRIEND_H
#define DEF_PROTECTFRIEND_H

#include"SubmissionRuleBase.h"

//���������悤�ɔz�u�ł��Ă��邩�]������
class ProtectFriend:public SubmissionRuleBase{
public:
	ProtectFriend()=default;
	~ProtectFriend()=default;
	SubmissionEvaluation InAdvanceDataEvaluate(const std::shared_ptr<InAdvanceCalculateDataBase> &,const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const;//�P�̍s���ɂ��ă��[�u���b�N�]��������
	std::string GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const;//�ł������������[�u���b�N�]���ɂ��āA�����I�ɂȃR�����g��Ԃ�
	std::string GetWholeLookBackActionEmpty()const;//���[���ɊY�������s���������Ȃ��������̑����I�]���̕������Ԃ�
	std::string GetSubmissionExplanation()const;//�T�u�~�b�V�����̐�������Ԃ�
	bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const;//�]�����s�����ǂ���
	std::string GetReason(SubmissionEvaluation rubric)const;//���[�u���b�N�]���̗��R�̐�������Ԃ�
};

#endif // !DEF_PROTECTFRIEND_H
