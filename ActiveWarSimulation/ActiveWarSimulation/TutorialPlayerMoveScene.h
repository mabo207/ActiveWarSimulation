#ifndef DEF_TUTORIALPLAYERMOVESCENE_H
#define DEF_TUTORIALPLAYERMOVESCENE_H

#include"PlayerMoveScene.h"
#include"TutorialBattleSceneData.h"

class TutorialPlayerMoveScene:public PlayerMoveScene{
	//Œ^E—ñ‹“‘Ì

	//’è”

	//•Ï”
protected:
	std::shared_ptr<TutorialBattleSceneData> m_tutorialBattleSceneData;

	//ŠÖ”
public:
	TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~TutorialPlayerMoveScene();

	void thisDraw()const;

};

#endif // !DEF_TUTORIALPLAYERMOVESCENE_H
