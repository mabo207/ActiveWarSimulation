#include"BaseUIInStageSelect.h"

BaseUIInStageSelect::ControledData::ControledData(size_t i_stageIndex,StageLevel i_selectLevel)
	:stageIndex(i_stageIndex)
	,selectLevel(i_selectLevel)
	,stageInfoPos()
	,levelInfoPos()
{}

BaseUIInStageSelect::BaseUIInStageSelect(const std::weak_ptr<ControledData> &controledData)
	:m_controledData(controledData)
{}

BaseUIInStageSelect::~BaseUIInStageSelect(){}