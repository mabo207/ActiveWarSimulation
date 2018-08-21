#ifndef DEF_STAGESELECTSCENE_H
#define DEF_STAGESELECTSCENE_H

#include"GameScene.h"
#include<vector>
#include<string>
#include"input.h"

class StageSelectScene:public GameScene{
	//�^�E�񋓑�
private:
	struct StageInfo{
		int m_mapPic;//�}�b�v�O���t�B�b�N(�k���\��)(push_back()�̍ۂɃf�X�g���N�^���Ă΂�O���t�B�b�N���������̂ŁA�폜�̓f�X�g���N�^�ł͍s��Ȃ��B�ǂ����Ă��f�X�g���N�^�ł������Ȃ�����A�R�s�[�R���X�g���N�^������čēxCopyGraph()�����悤�B)
		std::string m_dirName;//�f�B���N�g����
		std::string m_stageName;//�X�e�[�W��
		std::string m_explain;//�X�e�[�W������
		StageInfo(const int mapPic,const std::string &dirName,const std::string &stageName,const std::string &explain)
			:m_mapPic(mapPic),m_dirName(dirName),m_stageName(stageName),m_explain(explain){}
		~StageInfo();
	};

	//�萔

	//�ϐ�
protected:
	size_t m_selectStageIndex;//�I�𒆂̃X�e�[�W
	std::vector<StageInfo> m_stageInfoVec;//�X�e�[�W�ꗗ���
	std::shared_ptr<MainControledGameScene::RequiredInfoToMakeClass> *const m_pReqInfo;
	
	//�}�E�X�ŃN���b�N�ł���{�^���Q
	const MouseButtonUI m_beforeStageButton;
	const MouseButtonUI m_afterStageButton;
	const MouseButtonUI m_backButton;
	const MouseButtonUI m_playButton;

	//�O���t�B�b�N��
	const int m_backPic;
	const int m_stageNameFont;
	const int m_explainFont;
	
	//�֐�
protected:

public:
	StageSelectScene(std::shared_ptr<MainControledGameScene::RequiredInfoToMakeClass> *const pReqInfo);
	~StageSelectScene();
	int Calculate();
	void Draw()const;
};

#endif // !DEF_STAGESELECTSCENE_H

