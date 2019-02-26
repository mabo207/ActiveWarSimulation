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

	//関数
protected:
	void UpdateFlagOnlyInherit();//boolのメンバ変数m_(hogehoge)OnlyChangeInheritを、m_tutorialBattleSceneDataに従って更新する
	void GoNextTutorial();//次のチュートリアルデータへ更新
	bool TutorialMoveProcess(int retIntPal);//移動チュートリアルの判定と処理、遷移処理を行うかどうかを返す
	bool TutorialAttackProcess(int retIntPal);//攻撃チュートリアルの判定と処理、遷移処理を行うかどうかを返す
	bool TutorialWaitProcess(int retIntPal);//待機チュートリアルの判定と処理、遷移処理を行うかどうかを返す
	bool TutorialExplainProcess();//説明チュートリアルの判定と処理、遷移処理を行うかどうかを返す

public:
	TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~TutorialPlayerMoveScene();
	int thisCalculate();
	void thisDraw()const;

};

#endif // !DEF_TUTORIALPLAYERMOVESCENE_H
