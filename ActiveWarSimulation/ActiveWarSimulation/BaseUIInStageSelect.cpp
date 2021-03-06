#include"BaseUIInStageSelect.h"

//---------------------BaseUIInStageSelect---------------------
const int BaseUIInStageSelect::infoDrawAreaWidth=520;

BaseUIInStageSelect::ControledData::ControledData(size_t i_stageIndex,StageLevel i_selectLevel)
	:stageIndex(i_stageIndex)
	,selectLevel(i_selectLevel)
	,stageInfoPos()
	,levelInfoPos()
{}

BaseUIInStageSelect::BaseUIInStageSelect(const std::weak_ptr<ControledData> &controledData,const MouseButtonUI &backButton)
	:m_controledData(controledData)
	,m_backButton(backButton)
{}

BaseUIInStageSelect::~BaseUIInStageSelect(){}