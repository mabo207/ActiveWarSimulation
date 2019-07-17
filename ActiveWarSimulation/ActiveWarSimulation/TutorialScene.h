#ifndef DEF_TUTORIALSCENE_H
#define DEF_TUTORIALSCENE_H

#include"BattleScene.h"

class TutorialScene:public BattleScene{
	//列挙体・型
public:
	class TutorialSceneFactory:public SceneFactory{
	public:
		TutorialSceneFactory(const std::string &stageDirName);//ディレクトリ情報しか取得できていない場合
		TutorialSceneFactory(const std::string &stageDirName,const std::string &titleName);
		~TutorialSceneFactory();
		std::shared_ptr<GameScene> CreateIncompleteScene()const;
	private:
		const std::string m_stageDirName;
		std::string m_titleName;
	};

	//定数

	//変数
protected:
	TutorialScene(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel);

	//関数
public:
	~TutorialScene();

	//仮想関数のオーバーライド
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;
};

#endif // !DEF_TUTORIALSCENE_H
