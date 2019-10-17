#ifndef DEF_SUBMISSIONPLAYERMOVESCENE_H
#define DEF_SUBMISSIONPLAYERMOVESCENE_H

#include"TutorialPlayerMoveScene.h"

//サブミッション用のPlayerMoveScene
//TutorialPlayerMoveSceneを拡張する

class SubmissionPlayerMoveScene:public TutorialPlayerMoveScene{
public:
	SubmissionPlayerMoveScene(const std::shared_ptr<BattleSceneData> &battleSceneData);
	~SubmissionPlayerMoveScene();

protected:
	int thisCalculate();
	void thisDraw()const;

private:
	//サブミッション描画用
	size_t m_notOperateFrame;//何も操作していないフレーム時間
	PositionControl m_submissionPosition;//サブミッションの描画位置
};

#endif // !DEF_SUBMISSIONPLAYERMOVESCENE_H
