#ifndef DEF_SUBMISSIONREFLECTIONSCENE_H
#define DEF_SUBMISSIONREFLECTIONSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"

//�T�u�~�b�V�����ݒ莞�A���U���g��ʂ̑O�ɐU��Ԃ���s�����
class SubmissionReflectionScene:public BattleSceneElement{
public:
	SubmissionReflectionScene(const std::shared_ptr<BattleSceneData> &battleSceneData,const std::shared_ptr<BattleSceneElement> &clearScene);
	~SubmissionReflectionScene();

private:
	//�֐�
	void DrawResizedMap(int x,int y,const std::vector<Unit> &unitList,const std::shared_ptr<const LogElement> &log)const;//�ȈՊg��k���}�b�v��`�悷��
	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void thisDraw()const;
	int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	void ReturnProcess();//���̏�ʂɖ߂��Ă������̏���

	//�ϐ�
	const std::shared_ptr<BattleSceneData> m_battleSceneData;
	std::shared_ptr<BattleSceneElement> m_clearScene;//�O�N���X�ō쐬����ClearScene���ꎞ�I�ɕۑ����邽�߂̕ϐ�

	const int m_operateCursor;//���샆�j�b�g���w���������߂̃}�[�J�[
};

#endif // !DEF_SUBMISSIONREFLECTIONSCENE_H
