#ifndef DEF_SUBMISSIONBATTLESCENE_H
#define DEF_SUBMISSIONBATTLESCENE_H

#include"BattleScene.h"

class SubmissionBattleScene:public BattleScene{
	//列挙体・型
public:
	class SubmissionBattleSceneFactory:public SceneFactory{
	public:
		SubmissionBattleSceneFactory(const std::string &stageDirName,const std::string &titleName,const StageLevel level);
		~SubmissionBattleSceneFactory();
		std::shared_ptr<GameScene> CreateIncompleteScene()const;
	private:
		const std::string m_stageDirName;
		std::string m_titleName;
		StageLevel m_level;
	};

	//定数

	//変数
protected:
	SubmissionBattleScene(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel);
	std::shared_ptr<BattleSceneElement> VGetSwitchUnitScene()const;

	//関数
public:
	~SubmissionBattleScene();

	//仮想関数のオーバーライド
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;
};

#endif // !DEF_SUBMISSIONBATTLESCENE_H
