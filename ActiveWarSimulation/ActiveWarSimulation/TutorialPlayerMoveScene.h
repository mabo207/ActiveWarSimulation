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

	//特定地点の移動チュートリアルに必要なデータ
	bool m_moveTutorialFlag;
	const std::shared_ptr<Shape> m_moveTutorialArea;//どこに移動すれば良いか（移動ユニットの中心点がこの図形の中に入れば良い）
	//特定キャラの攻撃チュートリアルに必要なデータ
	bool m_attackTutorialFlag;
	const Unit *m_targetUnit;//誰を攻撃すれば良いか


	//関数
public:
	TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~TutorialPlayerMoveScene();
	int thisCalculate();
	void thisDraw()const;

};

#endif // !DEF_TUTORIALPLAYERMOVESCENE_H
