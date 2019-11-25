#ifndef DEF_ATTACKLOG_H
#define DEF_ATTACKLOG_H

#include"LogElement.h"
#include<vector>
#include"RouteInfo.h"
#include"Weapon.h"

class AttackLog:public LogElement{
public:
	AttackLog(const BattleSceneData * const battleData,const Unit * const aimedUnit,const std::vector<RouteInfo> &route);
	AttackLog(const std::vector<Unit *> &unitList,const Unit * const aimedUnit,const std::vector<RouteInfo> &route);
	virtual ~AttackLog(){}
	const Unit *GetAimedUnit()const{
		return m_aimedUnit;
	}
	const std::vector<RouteInfo> GetRoute()const{
		return m_route;
	}
	UnitLogData GetAimedUnitData()const;//GetAimedUnit()の上位互換っぽいが、処理が遅いのでできるだけ使わないようにしたい。
	Weapon::AttackInfo GetAttackInfo()const;

private:
	const Unit * const m_aimedUnit;
	//メモリ負担になる上にいるかよく分からない、そのうち使いたくなるかも。
	const std::vector<RouteInfo> m_route;
};

#endif // !DEF_WAITLOG_H