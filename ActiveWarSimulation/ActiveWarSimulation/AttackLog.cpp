#include"AttackLog.h"

AttackLog::AttackLog(const BattleSceneData * const battleData,const Unit * const aimedUnit)
	:LogElement(LogKind::e_attack,battleData)
	,m_aimedUnit(aimedUnit)
{}

Weapon::AttackInfo AttackLog::GetAttackInfo()const{
	const Unit *operateUnit=GetOperateUnitData().punit;
	if(operateUnit!=nullptr){
		return operateUnit->GetBattleStatus().weapon->GetAttackInfo(operateUnit,m_aimedUnit);
	}
	return Weapon::AttackInfo();
}