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

	//�֐�
protected:
	void UpdateFlagOnlyInherit();//bool�̃����o�ϐ�m_(hogehoge)OnlyChangeInherit���Am_tutorialBattleSceneData�ɏ]���čX�V����
	void GoNextTutorial();//���̃`���[�g���A���f�[�^�֍X�V
	bool TutorialMoveProcess(int retIntPal);//�ړ��`���[�g���A���̔���Ə����A�J�ڏ������s�����ǂ�����Ԃ�
	bool TutorialAttackProcess(int retIntPal);//�U���`���[�g���A���̔���Ə����A�J�ڏ������s�����ǂ�����Ԃ�
	bool TutorialWaitProcess(int retIntPal);//�ҋ@�`���[�g���A���̔���Ə����A�J�ڏ������s�����ǂ�����Ԃ�
	bool TutorialExplainProcess();//�����`���[�g���A���̔���Ə����A�J�ڏ������s�����ǂ�����Ԃ�

public:
	TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~TutorialPlayerMoveScene();
	int thisCalculate();
	void thisDraw()const;

};

#endif // !DEF_TUTORIALPLAYERMOVESCENE_H
