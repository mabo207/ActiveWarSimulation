#ifndef DEF_INITLOG_H
#define DEF_INITLOG_H

#include"LogElement.h"

//バトルの最初のユニットの状態を取るログ
class InitLog:public LogElement{
public:
	InitLog(const BattleSceneData * const battleData)
		:LogElement(LogKind::e_init,battleData){}
	virtual ~InitLog(){}
	//仮想関数のオーバーライド
	void OutputLog(std::ofstream &ofs,const std::map<const Unit *,size_t> &unitPtrToIndex,const char splitter,const char beginer,const char ender)const;
};

#endif // !DEF_INITLOG_H