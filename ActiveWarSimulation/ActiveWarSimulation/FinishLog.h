#ifndef DEF_FINISHLOG_H
#define DEF_FINISHLOG_H

#include"LogElement.h"

//バトル終了時のユニットの状態を取るログ
class FinishLog:public LogElement{
public:
	FinishLog(const std::shared_ptr<const BattleSceneData> battleData)
		:LogElement(LogKind::e_finish,battleData){}
	virtual ~FinishLog(){}
};

#endif // !DEF_FINISHLOG_H