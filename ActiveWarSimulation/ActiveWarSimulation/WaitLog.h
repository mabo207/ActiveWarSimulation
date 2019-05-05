#ifndef DEF_WAITLOG_H
#define DEF_WAITLOG_H

#include"LogElement.h"
#include"MoveScene.h"
#include<vector>

class WaitLog:public LogElement{
public:
	explicit WaitLog(const BattleSceneData * const battleData)
		:LogElement(LogKind::e_wait,battleData){}
	virtual ~WaitLog(){}

private:
	//���������S�ɂȂ��ɂ��邩�悭������Ȃ��A���̂����g�������Ȃ邩���B
	//const std::vector<MoveScene::RouteInfo> m_route;
};

#endif // !DEF_WAITLOG_H