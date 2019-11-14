#ifndef DEF_WAITLOG_H
#define DEF_WAITLOG_H

#include"LogElement.h"
#include<vector>
#include"RouteInfo.h"

class WaitLog:public LogElement{
public:
	WaitLog(const BattleSceneData * const battleData,const std::vector<RouteInfo> &route)
		:LogElement(LogKind::e_wait,battleData),m_route(route){}
	WaitLog(const std::vector<Unit *> &unitList,const std::vector<RouteInfo> &route)
		:LogElement(LogKind::e_wait,unitList),m_route(route){}
	virtual ~WaitLog(){}
	const std::vector<RouteInfo> GetRoute()const{
		return m_route;
	}

private:
	//メモリ負担になる上にいるかよく分からない、そのうち使いたくなるかも。
	const std::vector<RouteInfo> m_route;
};

#endif // !DEF_WAITLOG_H