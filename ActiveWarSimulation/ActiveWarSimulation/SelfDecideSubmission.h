#ifndef DEF_SELFDECIDESUBMISSION_H
#define DEF_SELFDECIDESUBMISSION_H

#include<memory>
#include<vector>
#include<string>
#include"SubmissionRuleBase.h"
#include<map>

struct BattleSceneData;

//���Ȑݒ�T�u�~�b�V����
//�ЂƂ܂��u���S�n�т���|�ōU������v�������Ɏ�������A�ėp���E�g���͌�ōs��
class SelfDecideSubmission{
public:
	SelfDecideSubmission();
	~SelfDecideSubmission();
	//�ώ@����
	bool JudgeEvaluatedOrder(const BattleSceneData * const battleData)const;
	//���[�u���b�N�`�����
	bool JudgeDrawRubric()const;
	//���[�u���b�N�]�����s���~�ς���
	void RubricEvaluate(const BattleSceneData * const battleData);
	//�����I�U��Ԃ������
	void WholeLookBack();
	//�~�b�V�������e��`�悷��
	void DrawSubmission(int x,int y)const;
	//�g�b�v�̃��[�u���b�N�]���̌��ʂ�`�悷��
	void DrawRubric(int centerX,int centerY)const;
	//�g�b�v�̃��[�u���b�N�]���ɂ��āA���ʂ̗��R��`�悷��
	void DrawReason(int x,int y)const;
	//�����I�U��Ԃ�̓��e��`�悷��
	void DrawWholeLookBack(int x,int y)const;
	//���[��������
	void InitRubric(const std::shared_ptr<SubmissionRuleBase> &rule);
	//���[�����擾����
	std::shared_ptr<const SubmissionRuleBase> GetRule()const{
		return m_rule;
	}
	//�]���ꗗ���擾����
	std::vector<std::pair<SubmissionEvaluation,std::shared_ptr<const LogElement>>> GetRubricList()const{
		//�f�[�^���傫���Ȃ肷���邱�Ƃ��Ȃ��p�ɂɂ��g���Ȃ��͂��Ȃ̂ŁA�R�s�[���ēn��
		return m_rubricList;
	}

	//�T�u�~�b�V�����`��̗̈�̑傫��
	static const int s_submissionWidth;
	static const int s_submissionHeight;
	//�]�����R�`��̗̈�̑傫��
	static const int s_reasonWidth;
	static const int s_reasonHeight;

private:
	std::shared_ptr<SubmissionRuleBase> m_rule;//���[�u���b�N�]���̃��[��
	std::vector<std::pair<SubmissionEvaluation,std::shared_ptr<const LogElement>>> m_rubricList;//���[�u���b�N�]���ƎQ�Ƃ������O�̃y�A�̈ꗗ
	std::string m_wholeComment;//�����I�U��Ԃ�ɂ�����R�����g
	std::map<SubmissionEvaluation,size_t> m_rubricFrequencyMap;//�퓬�I�����Ƀ��[�u���b�N�]���̉񐔂̈ꗗ���i�[����

	const int m_sentenceFont;//���͂�`�悷�邽�߂̃t�H���g
	const int m_rubricFont;//���[�u���b�N�]����`�悷�邽�߂̃t�H���g
};

#endif // !DEF_SELFDECIDESUBMISSION_H
