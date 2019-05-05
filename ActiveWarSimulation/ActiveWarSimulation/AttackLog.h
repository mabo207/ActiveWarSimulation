#ifndef DEF_ATTACKLOG_H
#define DEF_ATTACKLOG_H

#include"LogElement.h"
#include"MoveScene.h"
#include<vector>

class AttackLog:public LogElement{
public:
	AttackLog(const BattleSceneData * const battleData,const Unit * const aimedUnit);
	virtual ~AttackLog(){}
	const Unit *GetAimedUnit()const{
		return m_aimedUnit;
	}
	UnitLogData GetAimedUnitData()const;//GetAimedUnit()�̏�ʌ݊����ۂ����A�������x���̂łł��邾���g��Ȃ��悤�ɂ������B
	Weapon::AttackInfo GetAttackInfo()const;

private:
	const Unit * const m_aimedUnit;
	//���������S�ɂȂ��ɂ��邩�悭������Ȃ��A���̂����g�������Ȃ邩���B
	//const std::vector<MoveScene::RouteInfo> m_route;
};

#endif // !DEF_WAITLOG_H