#ifndef DEF_FINISHLOG_H
#define DEF_FINISHLOG_H

#include"LogElement.h"

//バトル終了時のユニットの状態を取るログ
class FinishLog:public LogElement{
public:
	FinishLog(const BattleSceneData * const battleData);
	virtual ~FinishLog(){}
	int GetClearTurn()const{
		return m_clearTurn;
	}

private:
	const int m_clearTurn;
};

#endif // !DEF_FINISHLOG_H