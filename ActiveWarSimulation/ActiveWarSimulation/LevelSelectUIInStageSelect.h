#ifndef DEF_LEVELSELECTUIINSTAGESELECT_H
#define DEF_LEVELSELECTUIINSTAGESELECT_H

#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"

//レベル選択をするUI
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
	void BecomeAfterTransitionAction(UpdateResult result);//項目をスライドアウトさせる設定処理

	const StageInfoInStageSelect m_stageInfo;
	Vector2D m_beforeFrameMousePos;//1フレーム前のマウスの位置
	Easing m_levelButtonX;
	PositionControl m_stageInfoCenterPos;
	std::array<MouseButtonUI,StageLevel::levelCount> m_levelButton;
	bool m_afterTransitionAction;//決定(または戻る)の後、少ししてから遷移するために用意
	UpdateResult m_transitionResult;//どこに遷移するかの情報

	//参照しているフォント（ここで削除はしない）
	const int m_stageNameFont;
	const int m_explainFont;
};

#endif // !DEF_LEVELSELECTUIINSTAGESELECT_H