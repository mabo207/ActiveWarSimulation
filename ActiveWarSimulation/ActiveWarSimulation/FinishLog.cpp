#include"FinishLog.h"

FinishLog::FinishLog(const BattleSceneData * const battleData)
	:LogElement(LogKind::e_finish,battleData)
	,m_clearTurn(battleData->CalculateTurn())
{}