#ifndef DEF_LEVELSELECTUIINSTAGESELECT_H
#define DEF_LEVELSELECTUIINSTAGESELECT_H

#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"

//���x���I��������UI
class LevelSelectUIInStageSelect:public BaseUIInStageSelect{
public:
	LevelSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData
		,const MouseButtonUI &backButton
		,const StageInfoInStageSelect &stageInfo
		,int stageNameFont
		,int explainFont
	);
	~LevelSelectUIInStageSelect();
	UpdateResult Update();
	void Draw()const;

private:
	void BecomeAfterTransitionAction(UpdateResult result);//���ڂ��X���C�h�A�E�g������ݒ菈��

	const StageInfoInStageSelect m_stageInfo;
	Vector2D m_beforeFrameMousePos;//1�t���[���O�̃}�E�X�̈ʒu
	Easing m_levelButtonX;
	PositionControl m_stageInfoCenterPos;
	std::array<MouseButtonUI,StageLevel::levelCount> m_levelButton;
	bool m_afterTransitionAction;//����(�܂��͖߂�)�̌�A�������Ă���J�ڂ��邽�߂ɗp��
	UpdateResult m_transitionResult;//�ǂ��ɑJ�ڂ��邩�̏��

	//�Q�Ƃ��Ă���t�H���g�i�����ō폜�͂��Ȃ��j
	const int m_stageNameFont;
	const int m_explainFont;
};

#endif // !DEF_LEVELSELECTUIINSTAGESELECT_H