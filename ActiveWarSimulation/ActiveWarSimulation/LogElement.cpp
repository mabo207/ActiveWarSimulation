#include"LogElement.h"
#include<algorithm>

const LogElement::UnitLogData LogElement::GetOperateUnitData()const{
	if(!m_unitDataList.empty()){
		return m_unitDataList[0];
	}
	return UnitLogData();
}

bool LogElement::JudgeEveryUnitData(const std::function<bool(const UnitLogData &)> &func,bool all)const{
	for(const UnitLogData &logData:m_unitDataList){
		if(func(logData)!=all){
			return !all;
		}
	}
	return all;
}

const LogElement::UnitLogData LogElement::FindUnitData(const Unit *punit)const{
	for(const UnitLogData &logData:m_unitDataList){
		if(logData.punit==punit){
			return logData;
		}
	}
	//見つからない場合はデフォルト値を返す
	return UnitLogData();
}

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