#ifndef DEF_TUTORIALSCENE_H
#define DEF_TUTORIALSCENE_H

#include"BattleScene.h"

class TutorialScene:public BattleScene{
	//�񋓑́E�^
public:
	class TutorialSceneFactory:public MainSceneFactory{
	public:
		TutorialSceneFactory(const std::string &stageDirName);
		~TutorialSceneFactory();
		std::shared_ptr<MainControledGameScene> CreateScene()const;
	private:
		const std::string m_stageDirName;
	};

	//�萔

	//�ϐ�

	//�֐�
public:
	TutorialScene(const std::string &stageName);
	~TutorialScene();
};

#endif // !DEF_TUTORIALSCENE_H
