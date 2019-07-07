#ifndef DEF_STAGEINFOINSTAGESELECT_H
#define DEF_STAGEINFOINSTAGESELECT_H

#include<string>
#include"ScoreRankingData.h"
#include"ToolsLib.h"

//�X�e�[�W�Z���N�g��ʂɂ�����X�e�[�W���
struct StageInfoInStageSelect{
	//�ϐ�
	int m_mapPic;//�}�b�v�O���t�B�b�N(�k���\��)(push_back()�̍ۂɃf�X�g���N�^���Ă΂�O���t�B�b�N���������̂ŁA�폜�̓f�X�g���N�^�ł͍s��Ȃ��B�ǂ����Ă��f�X�g���N�^�ł������Ȃ�����A�R�s�[�R���X�g���N�^������čēxCopyGraph()�����悤�B)
	std::string m_dirName;//�f�B���N�g����
	std::string m_titleName;//�X�e�[�W��
	std::string m_explain;//�X�e�[�W������
	ScoreRankingData::StageScoreData m_rankingVec;//�����L���O�f�[�^
	Vector2D m_pos;//�{�^���̈ʒu

	//�ÓI�ϐ�
	static const int boxWidth;
	static const int boxHeight;

	//�֐�
	StageInfoInStageSelect(const int mapPic,const std::string &dirName,const std::string &explain,const ScoreRankingData &rankingData);
	~StageInfoInStageSelect();
	void DrawInfo(const int centerX,const int centerY,const int nameFont,const int explainFont)const;//�X�e�[�W���ƃ}�b�v�G�A��������`�悷��
};


#endif // !DEF_STAGEINFOINSTAGESELECT_H