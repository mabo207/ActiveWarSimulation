#ifndef DEF_STAGESELECTUIINSTAGESELECT_H
#define DEF_STAGESELECTUIINSTAGESELECT_H

#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"

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
	Vector2D m_beforeFrameMousePos;//1フレーム前のマウスの位置
	bool m_drawStageInfo;//マウスを動かしてステージを示す丸の外に出したらステージ情報は描画しないようにする

	//参照しているフォント（ここで削除はしない）
	const int m_stageNameFont;
	const int m_explainFont;
};

#endif // !DEF_STAGESELECTUIINSTAGESELECT_H