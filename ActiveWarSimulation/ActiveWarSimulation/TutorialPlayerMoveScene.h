#ifndef DEF_TUTORIALPLAYERMOVESCENE_H
#define DEF_TUTORIALPLAYERMOVESCENE_H

#include"PlayerMoveScene.h"
#include"TutorialBattleSceneData.h"

class TutorialPlayerMoveScene:public PlayerMoveScene{
	//型・列挙体

	//定数

	//変数
protected:
	//チュートリアルデータ
	std::shared_ptr<TutorialBattleSceneData> m_tutorialBattleSceneData;

	int m_animeFlame;

	bool m_moveTutorialFlag;
	const std::shared_ptr<Shape> m_moveTutorialArea;

	//関数
public:
	TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~TutorialPlayerMoveScene();
	int thisCalculate();
	void thisDraw()const;

};

#endif // !DEF_TUTORIALPLAYERMOVESCENE_H
