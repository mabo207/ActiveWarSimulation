#ifndef DEF_STAGESELECTUIINSTAGESELECT_H
#define DEF_STAGESELECTUIINSTAGESELECT_H

#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"
#include"input.h"
#include"ToolsLib.h"

class StageSelectUIInStageSelect:public BaseUIInStageSelect{
public:
	StageSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData
		,const std::vector<StageInfoInStageSelect> &stageInfoVec
		,int stageNameFont
		,int explainFont
	);
	~StageSelectUIInStageSelect();
	UpdateResult Update();
	void Draw()const;

private:
	const std::vector<StageInfoInStageSelect> &m_stageInfoVec;
	const MouseButtonUI m_upButton,m_downButton;//�X�e�[�W���㉺�ɓ��������߂̃{�^��
	const MouseButtonUI m_selectStageButton;//�I��ł���X�e�[�W�̃{�^���i�����Ȃ��{�^���ł���j
	Vector2D m_beforeFrameMousePos;//1�t���[���O�̃}�E�X�̈ʒu
	PositionControl m_selectStagePos;//�I�����Ă���X�e�[�W���̕`��ʒu
	size_t m_beforeSelectStageIndex;//�X�e�[�W���̕`��ʒu�������Ă��鎞�A�������O�ɑI�����Ă����X�e�[�W��index���L�^����2�X�e�[�W�Ԃ̃X�e�[�W��S�ĕ`��ł���悤�ɂ���
	bool m_afterDicide;//true�Ȃ獀�ڂ��X���C�h�A�E�g����A�j���[�V���������Ă���

	//�Q�Ƃ��Ă���t�H���g�i�����ō폜�͂��Ȃ��j
	const int m_stageNameFont;
	const int m_explainFont;
};

#endif // !DEF_STAGESELECTUIINSTAGESELECT_H