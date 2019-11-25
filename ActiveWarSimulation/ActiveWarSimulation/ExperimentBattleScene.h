#ifndef DEF_EXPERIMENTBATTLESCENE_H
#define DEF_EXPERIMENTBATTLESCENE_H

#include"BattleScene.h"

//実験用のBattleScene、タイトルに戻るのとステージのレベルが固定される事以外に違いがない
class ExperimentBattleScene:public BattleScene{
public:
	class ExperimentBattleSceneFactory:public SceneFactory{
	public:
		ExperimentBattleSceneFactory(const std::string &stageDirName);
		~ExperimentBattleSceneFactory();
		std::shared_ptr<GameScene> CreateIncompleteScene()const;
	private:
		const std::string m_stageDirName;
	};

	virtual ~ExperimentBattleScene();

protected:
	ExperimentBattleScene(const std::string &stageDirName);
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;//タイトルに戻る
};

#endif // !DEF_EXPERIMENTBATTLESCENE_H
