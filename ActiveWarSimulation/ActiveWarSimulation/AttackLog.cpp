#include"AttackLog.h"

AttackLog::AttackLog(const std::shared_ptr<const BattleSceneData> battleData,const Unit * const aimedUnit)
	:LogElement(LogKind::e_attack,battleData)
	,m_aimedUnit(aimedUnit)
{}

Weapon::AttackInfo AttackLog::GetAttackInfo()const{
	const Unit *operateUnit=GetOperateUnit();
	if(operateUnit!=nullptr){
		return operateUnit->GetBattleStatus().weapon->GetAttackInfo(operateUnit,m_aimedUnit);
	}
	return Weapon::AttackInfo();
}