#ifndef DEF_STAGESELECTUIINSTAGESELECT_H
#define DEF_STAGESELECTUIINSTAGESELECT_H

#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"

class StageSelectUIInStageSelect:public BaseUIInStageSelect{
public:
	StageSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData,const size_t stageNum,const std::vector<StageInfoInStageSelect> &stageInfoVec);
	~StageSelectUIInStageSelect();
	UpdateResult Update();
	void Draw()const;
	std::shared_ptr<BaseUIInStageSelect> GetNextUI(const std::weak_ptr<ControledData> &controledData)const;

private:
	const size_t m_stageNum;
	const std::vector<StageInfoInStageSelect> &m_stageInfoVec;
};

#endif // !DEF_STAGESELECTUIINSTAGESELECT_H