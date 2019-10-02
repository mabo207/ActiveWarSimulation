#ifndef DEF_ARMERPOSITION_H
#define DEF_ARMERPOSITION_H

#include"SubmissionRuleBase.h"

//�d�����̑ҋ@�|�W�V�����ɂ��Ă̕]�����[��
class ArmerPosition:public SubmissionRuleBase{
public:
	ArmerPosition()=default;
	~ArmerPosition()=default;
	int RubricEvaluate(const BattleSceneData * const battleData)const;//�P�̍s���ɂ��ă��[�u���b�N�]��������
	std::pair<std::string,unsigned int> GetRubricStringInfo(int rubric)const;//���[�u���b�N�]���ɑ΂��镶�����`�悷�邽�߂̏���Ԃ��i�����񁕕����̉��̐F�j
	std::string GetWholeLookBack(int mostFrequentEvaluate)const;//�ł������������[�u���b�N�]���ɂ��āA�����I�ɂȃR�����g��Ԃ�
	std::string GetWholeLookBackActionEmpty()const;//���[���ɊY�������s���������Ȃ��������̑����I�]���̕������Ԃ�
	std::string GetSubmissionExplanation()const;//�T�u�~�b�V�����̐�������Ԃ�
	bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const;//�]�����s�����ǂ���
};

#endif // !DEF_ARMERPOSITION_H
