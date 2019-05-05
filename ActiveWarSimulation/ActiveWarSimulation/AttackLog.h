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
	Weapon::AttackInfo GetAttackInfo()const;

private:
	const Unit * const m_aimedUnit;
	//ƒƒ‚ƒŠ•‰’S‚É‚È‚éã‚É‚¢‚é‚©‚æ‚­•ª‚©‚ç‚È‚¢A‚»‚Ì‚¤‚¿g‚¢‚½‚­‚È‚é‚©‚àB
	//const std::vector<MoveScene::RouteInfo> m_route;
};

#endif // !DEF_WAITLOG_H