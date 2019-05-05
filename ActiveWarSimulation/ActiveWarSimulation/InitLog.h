#ifndef DEF_INITLOG_H
#define DEF_INITLOG_H

#include"LogElement.h"

//バトルの最初のユニットの状態を取るログ
class InitLog:public LogElement{
public:
	InitLog(const std::shared_ptr<const BattleSceneData> battleData)
		:LogElement(LogKind::e_init,battleData){}
	virtual ~InitLog(){}
};

#endif // !DEF_INITLOG_H