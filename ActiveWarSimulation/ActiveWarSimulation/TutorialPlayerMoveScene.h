#ifndef DEF_TUTORIALPLAYERMOVESCENE_H
#define DEF_TUTORIALPLAYERMOVESCENE_H

#include"PlayerMoveScene.h"
#include"TutorialBattleSceneData.h"

class TutorialPlayerMoveScene:public PlayerMoveScene{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	//�`���[�g���A���f�[�^
	std::shared_ptr<TutorialBattleSceneData> m_tutorialBattleSceneData;

	int m_animeFlame;

	bool m_moveTutorialFlag;
	const std::shared_ptr<Shape> m_moveTutorialArea;

	//�֐�
public:
	TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~TutorialPlayerMoveScene();
	int thisCalculate();
	void thisDraw()const;

};

#endif // !DEF_TUTORIALPLAYERMOVESCENE_H
