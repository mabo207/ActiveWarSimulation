#ifndef DEF_LEVELSELECTUIINSTAGESELECT_H
#define DEF_LEVELSELECTUIINSTAGESELECT_H

#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"
#include"MyPolygon.h"
#include"input.h"

//���x���I��������UI
class LevelSelectUIInStageSelect:public BaseUIInStageSelect{
public:
	LevelSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData
		,const StageInfoInStageSelect &stageInfo
		,int stageNameFont
		,int explainFont
	);
	~LevelSelectUIInStageSelect();
	UpdateResult Update();
	void Draw()const;

private:
	const StageInfoInStageSelect m_stageInfo;
	Vector2D m_beforeFrameMousePos;//1�t���[���O�̃}�E�X�̈ʒu
	Easing m_levelButtonX;
	PositionControl m_stageInfoCenterPos;
	std::array<MouseButtonUI,StageLevel::levelCount> m_levelButton;

	//�Q�Ƃ��Ă���t�H���g�i�����ō폜�͂��Ȃ��j
	const int m_stageNameFont;
	const int m_explainFont;
};

#endif // !DEF_LEVELSELECTUIINSTAGESELECT_H