#ifndef DEF_EXPERIMENTBATTLESCENE_H
#define DEF_EXPERIMENTBATTLESCENE_H

#include"BattleScene.h"

//�����p��BattleScene�A�^�C�g���ɖ߂�̂ƃX�e�[�W�̃��x�����Œ肳��鎖�ȊO�ɈႢ���Ȃ�
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
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;//�^�C�g���ɖ߂�
};

#endif // !DEF_EXPERIMENTBATTLESCENE_H
