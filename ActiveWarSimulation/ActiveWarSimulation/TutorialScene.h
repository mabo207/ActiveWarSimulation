#ifndef DEF_TUTORIALSCENE_H
#define DEF_TUTORIALSCENE_H

#include"BattleScene.h"

class TutorialScene:public BattleScene{
	//—ñ‹“‘ÌEŒ^
public:
	class TutorialSceneFactory:public MainSceneFactory{
	public:
		TutorialSceneFactory(const std::string &stageDirName);
		~TutorialSceneFactory();
		std::shared_ptr<MainControledGameScene> CreateScene()const;
	private:
		const std::string m_stageDirName;
	};

	//’è”

	//•Ï”
protected:
	TutorialScene(const std::string &stageName);

	//ŠÖ”
public:
	~TutorialScene();
};

#endif // !DEF_TUTORIALSCENE_H
