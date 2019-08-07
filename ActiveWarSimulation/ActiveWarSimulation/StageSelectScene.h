#ifndef DEF_STAGESELECTSCENE_H
#define DEF_STAGESELECTSCENE_H

#include"GameScene.h"
#include<vector>
#include<string>
#include<map>
#include"input.h"
#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"
#include"BGM.h"

class StageSelectScene:public GameScene{
	//�^�E�񋓑�
private:
	//���̏�ʂ����ɂ��邩
	enum class NextSceneName{
		e_title
		,e_battle
	};

public:
	class StageSelectSceneFactory:public SceneFactory{
		//�N���X�����̂ɓ��ɕK�v�ȃf�[�^�͂Ȃ�
	public:
		StageSelectSceneFactory():SceneFactory(){}
		~StageSelectSceneFactory(){}
		std::shared_ptr<GameScene> CreateIncompleteScene()const;
	};

	//�萔

	//�ϐ�
protected:
	std::vector<StageInfoInStageSelect> m_stageInfoVec;//�X�e�[�W�ꗗ���
	NextSceneName m_nextSceneName;//���̏�ʂ͉����AVGetNextScene()�Ŏg�p
	std::shared_ptr<BaseUIInStageSelect> m_ui;//���݂�UI
	std::shared_ptr<BaseUIInStageSelect::ControledData> m_uiControledData;//UI���Ǘ�����f�[�^

	//�摜
	const int m_backDefaultPic;//�w�i�i�f�t�H���g�j
	const int m_backNightPic;//�w�i�i��j
	const int m_backMorningPic;//�w�i�i���Ă��j

	//�{�^��
	const MouseButtonUI m_backButton;

	//�t�H���g
	const int m_stageNameFont;
	const int m_explainFont;
	
	//BGM
	const Resource::BGM m_bgm;

	//���ʉ�

private:
	std::map<std::string,int> m_stageInfoFactoryMap;//�X�e�[�W���̃��[�h�̂��߂̏��(�~�j�}�b�v�ɂ��Ă͕ʃX���b�h���������邽�߂�StageInfoInStageSelect�N���X�Œ��ڃ��[�h�����ɂ��̕ϐ��Ƀ��[�h����)

	//�֐�
protected:
	StageSelectScene();
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;
	void DrawBack()const;//�w�i�̕`��

public:
	~StageSelectScene();
	void InitCompletely();
	void Activate();
	int Calculate();
	void Draw()const;
};

#endif // !DEF_STAGESELECTSCENE_H

