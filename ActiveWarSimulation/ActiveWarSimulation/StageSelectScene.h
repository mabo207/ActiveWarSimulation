#ifndef DEF_STAGESELECTSCENE_H
#define DEF_STAGESELECTSCENE_H

#include"GameScene.h"
#include<vector>
#include<string>
#include"input.h"
#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"

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
		std::shared_ptr<GameScene> CreateScene()const;
	};

	//�萔

	//�ϐ�
protected:
	std::vector<StageInfoInStageSelect> m_stageInfoVec;//�X�e�[�W�ꗗ���
	NextSceneName m_nextSceneName;//���̏�ʂ͉����AVGetNextScene()�Ŏg�p
	std::shared_ptr<BaseUIInStageSelect> m_ui;//���݂�UI
	std::shared_ptr<BaseUIInStageSelect::ControledData> m_uiControledData;//UI���Ǘ�����f�[�^
	
	//�O���t�B�b�N��
	const int m_backPic;
	const int m_stageNameFont;
	const int m_explainFont;
	
	//���ʉ�

	//�֐�
protected:
	StageSelectScene();
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;

public:
	~StageSelectScene();
	int Calculate();
	void Draw()const;
};

#endif // !DEF_STAGESELECTSCENE_H

