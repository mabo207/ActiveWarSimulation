#ifndef DEF_LEVELSELECTUIINSTAGESELECT_H
#define DEF_LEVELSELECTUIINSTAGESELECT_H

#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"
#include"MyPolygon.h"
#include"input.h"

//レベル選択をするUI
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
	Vector2D m_beforeFrameMousePos;//1フレーム前のマウスの位置
	Easing m_levelButtonX;
	PositionControl m_stageInfoCenterPos;
	std::array<MouseButtonUI,StageLevel::levelCount> m_levelButton;

	//参照しているフォント（ここで削除はしない）
	const int m_stageNameFont;
	const int m_explainFont;
};

#endif // !DEF_LEVELSELECTUIINSTAGESELECT_H