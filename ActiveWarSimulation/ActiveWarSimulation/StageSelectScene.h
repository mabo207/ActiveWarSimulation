#ifndef DEF_STAGESELECTSCENE_H
#define DEF_STAGESELECTSCENE_H

#include"GameScene.h"
#include<vector>
#include<string>
#include"input.h"
#include"TitleScene.h"

class StageSelectScene:public GameScene{
	//�^�E�񋓑�
private:
	struct StageInfo{
		//�ϐ�
		int m_mapPic;//�}�b�v�O���t�B�b�N(�k���\��)(push_back()�̍ۂɃf�X�g���N�^���Ă΂�O���t�B�b�N���������̂ŁA�폜�̓f�X�g���N�^�ł͍s��Ȃ��B�ǂ����Ă��f�X�g���N�^�ł������Ȃ�����A�R�s�[�R���X�g���N�^������čēxCopyGraph()�����悤�B)
		std::string m_dirName;//�f�B���N�g����
		std::string m_stageName;//�X�e�[�W��
		int m_level;//���x��
		std::string m_explain;//�X�e�[�W������

		//�֐�
		StageInfo(const int mapPic,const std::string &dirName,const std::string &stageInfo,const std::string &explain);
		~StageInfo();
		std::string GetLevelStr()const;//��Փx��������镶�����Ԃ�
	};

	//�萔

	//�ϐ�
protected:
	size_t m_selectStageIndex;//�I�𒆂̃X�e�[�W
	std::vector<StageInfo> m_stageInfoVec;//�X�e�[�W�ꗗ���
	std::weak_ptr<TitleScene::SharedData> m_sharedData;//�^�C�g���V�[���̋��L���
	
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

public:
	StageSelectScene(const std::weak_ptr<TitleScene::SharedData> &sharedData);
	~StageSelectScene();
	int Calculate();
	void Draw()const;
};

#endif // !DEF_STAGESELECTSCENE_H

