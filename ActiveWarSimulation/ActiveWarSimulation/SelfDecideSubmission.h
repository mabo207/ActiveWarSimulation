#ifndef DEF_SELFDECIDESUBMISSION_H
#define DEF_SELFDECIDESUBMISSION_H

#include<memory>
#include<vector>
#include<string>

struct BattleSceneData;

//���Ȑݒ�T�u�~�b�V����
//�ЂƂ܂��u���S�n�т���|�ōU������v�������Ɏ�������A�ėp���E�g���͌�ōs��
class SelfDecideSubmission{
public:
	SelfDecideSubmission();
	~SelfDecideSubmission();
	//�ώ@����
	bool JudgeEvaluatedOrder(const BattleSceneData * const battleData)const;
	//���[�u���b�N�]�����s���~�ς���
	void RubricEvaluate(const BattleSceneData * const battleData);
	//�����I�U��Ԃ������
	void WholeLookBack();
	//�~�b�V�������e��`�悷��
	void DrawSubmission(int x,int y)const;
	//�g�b�v�̃��[�u���b�N�]���̌��ʂ�`�悷��
	void DrawRubric(int x,int y)const;
	//�����I�U��Ԃ�̓��e��`�悷��
	void DrawWholeLookBack(int x,int y)const;

private:
	std::vector<int> m_rubricList;//���[�u���b�N�]���ꗗ
	std::string m_wholeComment;//�����I�U��Ԃ�ɂ�����R�����g

	const int m_sentenceFont;//���͂�`�悷�邽�߂̃t�H���g
	const int m_rubricFont;//���[�u���b�N�]����`�悷�邽�߂̃t�H���g
};

#endif // !DEF_SELFDECIDESUBMISSION_H
