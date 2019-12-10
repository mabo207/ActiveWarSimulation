#include"FinishLog.h"
#include"BattleSceneData.h"

FinishLog::FinishLog(const BattleSceneData * const battleData)
	:LogElement(LogKind::e_finish,battleData)
	,m_clearTurn(battleData->CalculateTurn())
{}

FinishLog::FinishLog(const std::vector<UnitLogData> &unitDataList,int clearTurn)
	:LogElement(LogKind::e_finish,unitDataList)
	,m_clearTurn(clearTurn)
{}

void FinishLog::OutputLog(std::ofstream &ofs,const std::map<const Unit *,size_t> &unitPtrToIndex,const char splitter,const char beginer,const char ender)const{
	ofs<<beginer;
	ofs<<beginer<<"kind"<<splitter<<"FinishLog"<<ender;
	ofs<<splitter<<beginer<<"unitList"<<splitter;
	OutputUnitDataList(ofs,unitPtrToIndex,splitter,beginer,ender);
	ofs<<ender;
	ofs<<ender;
}
