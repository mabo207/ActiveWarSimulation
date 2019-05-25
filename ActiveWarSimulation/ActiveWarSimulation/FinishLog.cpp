#include"FinishLog.h"
#include"BattleSceneData.h"

FinishLog::FinishLog(const BattleSceneData * const battleData)
	:LogElement(LogKind::e_finish,battleData)
	,m_clearTurn(battleData->CalculateTurn())
{}