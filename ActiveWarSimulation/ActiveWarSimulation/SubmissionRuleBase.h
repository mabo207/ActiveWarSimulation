#ifndef DEF_SUBMISSIONRULEBASE_H
#define DEF_SUBMISSIONRULEBASE_H

#include<string>

struct BattleSceneData;

//�T�u�~�b�V�����]�����̃��[����\�����邽�߂̊��N���X
class SubmissionRuleBase{
public:
	virtual ~SubmissionRuleBase()=default;
	virtual int RubricEvaluate(const BattleSceneData * const battleData)const=0;//�P�̍s���ɂ��ă��[�u���b�N�]��������
	virtual std::pair<std::string,unsigned int> GetRubricStringInfo(int rubric)const=0;//���[�u���b�N�]���ɑ΂��镶�����`�悷�邽�߂̏���Ԃ��i�����񁕕����̉��̐F�j
	virtual std::string GetWholeLookBack(int mostFrequentEvaluate)const=0;//�ł������������[�u���b�N�]���ɂ��āA�����I�ɂȃR�����g��Ԃ�
	virtual std::string GetWholeLookBackActionEmpty()const=0;//���[���ɊY�������s���������Ȃ��������̑����I�]���̕������Ԃ�
	virtual std::string GetSubmissionExplanation()const=0;//�T�u�~�b�V�����̐�������Ԃ�
	virtual bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const=0;//�]�����s�����ǂ���

protected:
	SubmissionRuleBase()=default;
};

#endif // !DEF_SUBMISSIONRULEBASE_H
