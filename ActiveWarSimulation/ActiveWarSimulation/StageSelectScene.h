#ifndef DEF_STAGESELECTSCENE_H
#define DEF_STAGESELECTSCENE_H

#include"GameScene.h"
#include<vector>
#include<string>
#include"input.h"
#include"ScoreRankingData.h"

class StageSelectScene:public GameScene{
	//�^�E�񋓑�
private:
	//�X�e�[�W���
	struct StageInfo{
		//�ϐ�
		int m_mapPic;//�}�b�v�O���t�B�b�N(�k���\��)(push_back()�̍ۂɃf�X�g���N�^���Ă΂�O���t�B�b�N���������̂ŁA�폜�̓f�X�g���N�^�ł͍s��Ȃ��B�ǂ����Ă��f�X�g���N�^�ł������Ȃ�����A�R�s�[�R���X�g���N�^������čēxCopyGraph()�����悤�B)
		std::string m_dirName;//�f�B���N�g����
		std::string m_titleName;//�X�e�[�W��
		StageLevel m_level;//���x��
		std::string m_explain;//�X�e�[�W������
		std::vector<ScoreRankingData::PlayerData> m_rankingVec;//�����L���O�f�[�^

		//�֐�
		StageInfo(const int mapPic,const std::string &dirName,const std::string &explain,const ScoreRankingData &rankingData);
		~StageInfo();
		std::string GetLevelStr()const;//��Փx��������镶�����Ԃ�
	};
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
	size_t m_selectStageIndex;//�I�𒆂̃X�e�[�W
	std::vector<StageInfo> m_stageInfoVec;//�X�e�[�W�ꗗ���
	NextSceneName m_nextSceneName;//���̏�ʂ͉����AVGetNextScene()�Ŏg�p
	
	//�}�E�X�ŃN���b�N�ł���{�^���Q
	const MouseButtonUI m_beforeStageButton;
	const MouseButtonUI m_afterStageButton;
	const MouseButtonUI m_backButton;
	const MouseButtonUI m_playButton;

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

