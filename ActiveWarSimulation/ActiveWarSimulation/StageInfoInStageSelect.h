#ifndef DEF_STAGEINFOINSTAGESELECT_H
#define DEF_STAGEINFOINSTAGESELECT_H

#include<string>
#include"ScoreRankingData.h"
#include"ToolsLib.h"

//�X�e�[�W�Z���N�g��ʂɂ�����X�e�[�W���
struct StageInfoInStageSelect{
	//�ϐ�
	int m_mapPicRef;//�}�b�v�O���t�B�b�N(�k���\��)(�Q��)
	std::string m_dirName;//�f�B���N�g����
	std::string m_titleName;//�X�e�[�W��
	std::string m_explain;//�X�e�[�W������
	ScoreRankingData::StageScoreData m_rankingVec;//�����L���O�f�[�^
	Vector2D m_pos;//�{�^���̈ʒu

	//�ÓI�ϐ�
	static const int stageBoxWidth;
	static const int stageBoxHeight;
	static const int levelBoxWidth;
	static const int levelBoxHeight;

	//�֐�
	StageInfoInStageSelect(const int mapPic,const std::string &dirName,const std::string &explain,const ScoreRankingData &rankingData);
	~StageInfoInStageSelect();
	void DrawStageInfo(const int centerX,const int centerY,const int nameFont,const int explainFont)const;//�X�e�[�W���ƃ}�b�v�G�A��������`�悷��
	void DrawLevelInfo(const StageLevel level,const int x,const int y,const int levelNameFont,const int rankingFont)const;//�w�肳�ꂽ���x���ɂ��āA���x��������ƃ����L���O��\��
};


#endif // !DEF_STAGEINFOINSTAGESELECT_H