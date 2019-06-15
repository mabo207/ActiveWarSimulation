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

	//�Q�Ƃ��Ă���t�H���g�i�����ō폜�͂��Ȃ��j
	const int m_stageNameFont;
	const int m_explainFont;
};

#endif // !DEF_STAGESELECTUIINSTAGESELECT_H