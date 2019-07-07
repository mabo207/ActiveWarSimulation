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
	const MouseButtonUI m_upButton,m_downButton;//ステージを上下に動かすためのボタン
	const MouseButtonUI m_selectStageButton;//選んでいるステージのボタン（見えないボタンである）
	Vector2D m_beforeFrameMousePos;//1フレーム前のマウスの位置
	PositionControl m_selectStagePos;//選択しているステージ情報の描画位置
	size_t m_beforeSelectStageIndex;//ステージ情報の描画位置が動いている時、動かす前に選択していたステージのindexを記録して2ステージ間のステージを全て描画できるようにする
	bool m_afterDicide;//trueなら項目がスライドアウトするアニメーションをしている

	//参照しているフォント（ここで削除はしない）
	const int m_stageNameFont;
	const int m_explainFont;
};

#endif // !DEF_STAGESELECTUIINSTAGESELECT_H