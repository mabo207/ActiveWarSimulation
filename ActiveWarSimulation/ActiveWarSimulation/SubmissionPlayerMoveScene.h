#ifndef DEF_SUBMISSIONPLAYERMOVESCENE_H
#define DEF_SUBMISSIONPLAYERMOVESCENE_H

#include"TutorialPlayerMoveScene.h"

//�T�u�~�b�V�����p��PlayerMoveScene
//TutorialPlayerMoveScene���g������

class SubmissionPlayerMoveScene:public TutorialPlayerMoveScene{
public:
	SubmissionPlayerMoveScene(const std::shared_ptr<BattleSceneData> &battleSceneData);
	~SubmissionPlayerMoveScene();

protected:
	int thisCalculate();
	void thisDraw()const;

private:
	//�T�u�~�b�V�����`��p
	size_t m_notOperateFrame;//�������삵�Ă��Ȃ��t���[������
	PositionControl m_submissionPosition;//�T�u�~�b�V�����̕`��ʒu
};

#endif // !DEF_SUBMISSIONPLAYERMOVESCENE_H
