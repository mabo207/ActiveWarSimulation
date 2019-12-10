#include"WaitLog.h"
#include"BattleSceneData.h"

void WaitLog::OutputLog(std::ofstream &ofs,const std::map<const Unit *,size_t> &unitPtrToIndex,const char splitter,const char beginer,const char ender)const{
	ofs<<beginer;
	ofs<<beginer<<"kind"<<splitter<<"WaitLog"<<ender;
	ofs<<splitter<<beginer<<"unitList"<<splitter;
	OutputUnitDataList(ofs,unitPtrToIndex,splitter,beginer,ender);
	ofs<<ender;
	ofs<<ender;
}
