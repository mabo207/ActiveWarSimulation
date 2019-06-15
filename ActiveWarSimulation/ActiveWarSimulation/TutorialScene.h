#ifndef DEF_TUTORIALSCENE_H
#define DEF_TUTORIALSCENE_H

#include"BattleScene.h"

class TutorialScene:public BattleScene{
	//�񋓑́E�^
public:
	class TutorialSceneFactory:public SceneFactory{
	public:
		TutorialSceneFactory(const std::string &stageDirName);//�f�B���N�g����񂵂��擾�ł��Ă��Ȃ��ꍇ
		TutorialSceneFactory(const std::string &stageDirName,const std::string &titleName);
		~TutorialSceneFactory();
		std::shared_ptr<GameScene> CreateScene()const;
	private:
		const std::string m_stageDirName;
		std::string m_titleName;
	};

	//�萔

	//�ϐ�
protected:
	TutorialScene(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel);

	//�֐�
public:
	~TutorialScene();
};

#endif // !DEF_TUTORIALSCENE_H
