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

	//����n�_�̈ړ��`���[�g���A���ɕK�v�ȃf�[�^
	bool m_moveTutorialFlag;
	const std::shared_ptr<Shape> m_moveTutorialArea;//�ǂ��Ɉړ�����Ηǂ����i�ړ����j�b�g�̒��S�_�����̐}�`�̒��ɓ���Ηǂ��j
	//����L�����̍U���`���[�g���A���ɕK�v�ȃf�[�^
	bool m_attackTutorialFlag;
	const Unit *m_targetUnit;//�N���U������Ηǂ���


	//�֐�
public:
	TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~TutorialPlayerMoveScene();
	int thisCalculate();
	void thisDraw()const;

};

#endif // !DEF_TUTORIALPLAYERMOVESCENE_H
