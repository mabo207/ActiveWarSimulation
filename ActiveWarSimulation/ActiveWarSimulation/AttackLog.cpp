#include"AttackLog.h"
#include"BattleSceneData.h"

AttackLog::AttackLog(const BattleSceneData * const battleData,const Unit * const aimedUnit)
	:LogElement(LogKind::e_attack,battleData)
	,m_aimedUnit(aimedUnit)
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