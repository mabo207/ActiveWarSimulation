#ifndef DEF_SUBMISSIONSWITCHUNITSCENE_H
#define DEF_SUBMISSIONSWITCHUNITSCENE_H

//サブミッションを設定する必要がある場合のトップに設置するSwitchUnitScene
//インスタンス作成時に、m_nextSceneにSelectSubmissionSceneを生成するだけで、他の機能は変わらない。

#include"SwitchUnitScene.h"

class SubmissionSwitchUnitScene:public SwitchUnitScene{
public:
	SubmissionSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~SubmissionSwitchUnitScene();
};

#endif // !DEF_SUBMISSIONSWITCHUNITSCENE_H
