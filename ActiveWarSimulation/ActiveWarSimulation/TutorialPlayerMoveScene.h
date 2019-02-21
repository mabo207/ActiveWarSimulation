#ifndef DEF_TUTORIALPLAYERMOVESCENE_H
#define DEF_TUTORIALPLAYERMOVESCENE_H

#include"PlayerMoveScene.h"
#include"TutorialBattleSceneData.h"

class TutorialPlayerMoveScene:public PlayerMoveScene{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	std::shared_ptr<TutorialBattleSceneData> m_tutorialBattleSceneData;

	//�֐�
public:
	TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~TutorialPlayerMoveScene();

	void thisDraw()const;

};

#endif // !DEF_TUTORIALPLAYERMOVESCENE_H
