#ifndef DEF_ATTACKLOG_H
#define DEF_ATTACKLOG_H

#include"LogElement.h"
#include"MoveScene.h"
#include<vector>

class AttackLog:public LogElement{
public:
	AttackLog(const std::shared_ptr<const BattleSceneData> battleData,const Unit * const aimedUnit);
	virtual ~AttackLog(){}
	const Unit *GetAimedUnit()const{
		return m_aimedUnit;
	}
	Weapon::AttackInfo GetAttackInfo()const;

private:
	const Unit * const m_aimedUnit;
	//メモリ負担になる上にいるかよく分からない、そのうち使いたくなるかも。
	//const std::vector<MoveScene::RouteInfo> m_route;
};

#endif // !DEF_WAITLOG_H