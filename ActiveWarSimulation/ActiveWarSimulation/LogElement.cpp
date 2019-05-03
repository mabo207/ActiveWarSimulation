#include"LogElement.h"
#include<algorithm>

LogElement::LogElement(LogKind kind,const BattleSceneData * const battleData)
	:m_kind(kind)
	,m_unitDataList(CreateUnitDataList(battleData))
{}

std::vector<LogElement::UnitLogData> LogElement::CreateUnitDataList(const BattleSceneData * const battleData){
	std::vector<UnitLogData> retVec;
	const size_t vecSize=battleData->m_unitList.size();
	retVec.reserve(vecSize);
	for(size_t i=0;i<vecSize;i++){
		retVec.push_back(UnitLogData(battleData->m_unitList[i]));
	}
	return retVec;
}