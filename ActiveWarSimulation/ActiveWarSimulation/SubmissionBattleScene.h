#ifndef DEF_SUBMISSIONBATTLESCENE_H
#define DEF_SUBMISSIONBATTLESCENE_H

#include"BattleScene.h"

class SubmissionBattleScene:public BattleScene{
	//�񋓑́E�^
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

	//�萔

	//�ϐ�
protected:
	SubmissionBattleScene(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel);
	std::shared_ptr<BattleSceneElement> VGetSwitchUnitScene()const;

	//�֐�
public:
	~SubmissionBattleScene();

	//���z�֐��̃I�[�o�[���C�h
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;
};

#endif // !DEF_SUBMISSIONBATTLESCENE_H
