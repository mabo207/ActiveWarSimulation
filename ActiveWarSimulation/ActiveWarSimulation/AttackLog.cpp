#include"AttackLog.h"
#include"BattleSceneData.h"

AttackLog::AttackLog(const BattleSceneData * const battleData,const Unit * const aimedUnit,const std::vector<RouteInfo> &route)
	:LogElement(LogKind::e_attack,battleData)
	,m_aimedUnit(aimedUnit)
	,m_route(route)
{}

AttackLog::AttackLog(const std::vector<Unit *> &unitList,const Unit * const aimedUnit,const std::vector<RouteInfo> &route)
	:LogElement(LogKind::e_attack,unitList)
	,m_aimedUnit(aimedUnit)
	,m_route(route)
{}

LogElement::UnitLogData AttackLog::GetAimedUnitData()const{
	for(const UnitLogData &logData:m_unitDataList){
		if(logData.punit==m_aimedUnit){
			return logData;
		}
	}
	return UnitLogData();
}

Weapon::AttackInfo AttackLog::GetAttackInfo()const{
	const Unit *operateUnit=GetOperateUnitData().punit;
	if(operateUnit!=nullptr){
		return operateUnit->GetBattleStatus().weapon->GetAttackInfo(operateUnit,m_aimedUnit);
	}
	return Weapon::AttackInfo();
}