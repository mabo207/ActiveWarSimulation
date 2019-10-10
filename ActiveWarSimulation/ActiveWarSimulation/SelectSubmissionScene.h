#ifndef DEF_SELECTSUBMISSIONSCENE_H
#define DEF_SELECTSUBMISSIONSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include"MyPolygon.h"
#include<array>
#include"SubmissionRuleBase.h"
#include<memory>

//�T�u�~�b�V�����ݒ���

class SelectSubmissionScene:public BattleSceneElement{
public:
	explicit SelectSubmissionScene(const std::shared_ptr<BattleSceneData> &battleSceneData);
	~SelectSubmissionScene();

protected:
	//���z�֐��I�[�o�[���C�h
	int thisCalculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void thisDraw()const;
	int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	void ReturnProcess();//���̏�ʂɖ߂��Ă������̏���

private:
	//�ÓI�ϐ�
	static const size_t submissionCount=4;//�I���ł���T�u�~�b�V�����̌�
	static const std::array<MyPolygon,submissionCount> submissionHitJudgeShape;
	//���I�ϐ�
	const std::shared_ptr<BattleSceneData> m_battleSceneData;//�o�g���̃f�[�^
	size_t m_selectIndex;//�I�����Ă���T�u�~�b�V�����̔z��ԍ�
	std::array<std::shared_ptr<SubmissionRuleBase>,submissionCount> m_submissionList;
};

#endif // !DEF_SELECTSUBMISSIONSCENE_H
