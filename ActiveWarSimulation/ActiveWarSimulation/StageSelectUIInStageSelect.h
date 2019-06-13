#ifndef DEF_STAGESELECTUIINSTAGESELECT_H
#define DEF_STAGESELECTUIINSTAGESELECT_H

#include"BaseUIInStageSelect.h"

class StageSelectUIInStageSelect:public BaseUIInStageSelect{
public:
	StageSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData,const size_t stageNum);
	~StageSelectUIInStageSelect();
	UpdateResult Update();
	void Draw()const;
	std::shared_ptr<BaseUIInStageSelect> GetNextUI(const std::weak_ptr<ControledData> &controledData)const;

private:
	const size_t m_stageNum;
};

#endif // !DEF_STAGESELECTUIINSTAGESELECT_H