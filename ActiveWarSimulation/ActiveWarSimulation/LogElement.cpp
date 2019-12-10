#include"LogElement.h"
#include<algorithm>
#include"BattleSceneData.h"

LogElement::UnitLogData::UnitLogData(const Unit * const punit)
	:UnitLogData(punit->GetBattleStatus().HP
		,punit->GetBattleStatus().OP
		,punit->getPos()
		,punit)
{}

bool LogElement::UnitLogData::operator==(const UnitLogData &otherObj)const{
	return (this->hp==otherObj.hp
		&& this->op==otherObj.op
		&& this->pos==otherObj.pos
		&& this->punit==otherObj.punit);
}

const LogElement::UnitLogData LogElement::GetOperateUnitData()const{
	if(!m_unitDataList.empty()){
		return m_unitDataList[0];
	}
	return UnitLogData();
}

bool LogElement::JudgeEveryUnitData(const std::function<bool(const UnitLogData &)> &func,bool all)const{
	for(const UnitLogData &logData:m_unitDataList){
		if(func(logData)!=all){
			return !all;
		}
	}
	return all;
}

const LogElement::UnitLogData LogElement::FindUnitData(const Unit *punit)const{
	for(const UnitLogData &logData:m_unitDataList){
		if(logData.punit==punit){
			return logData;
		}
	}
	//見つからない場合はデフォルト値を返す
	return UnitLogData();
}

LogElement::LogElement(LogKind kind,const BattleSceneData * const battleData)
	:LogElement(kind,battleData->m_unitList)
{}

LogElement::LogElement(LogKind kind,const std::vector<Unit *> &unitList)
	:m_kind(kind)
	,m_unitDataList(CreateUnitDataList(unitList))
{}

void LogElement::OutputUnitDataList(std::ofstream &ofs,const std::map<const Unit *,size_t> &unitPtrToIndex,const char splitter,const char beginer,const char ender)const{
	const size_t listSize=m_unitDataList.size();
	for(size_t i=0;i<listSize;i++){
		ofs<<beginer;
		//情報出力
		ofs<<beginer<<"index"<<splitter<<unitPtrToIndex.find(m_unitDataList[i].punit)->second<<ender;
		ofs<<splitter<<beginer<<"hp"<<splitter<<m_unitDataList[i].hp<<ender;
		ofs<<splitter<<beginer<<"op"<<splitter<<m_unitDataList[i].op<<ender;
		ofs<<splitter<<beginer<<"pos"<<splitter<<m_unitDataList[i].pos.x<<splitter<<m_unitDataList[i].pos.y<<ender;
		//区切り文字出力
		ofs<<ender;
		if(i+1!=listSize){
			ofs<<splitter;
		}
	}
}

std::vector<LogElement::UnitLogData> LogElement::CreateUnitDataList(const std::vector<Unit *> &unitList){
	std::vector<UnitLogData> retVec;
	const size_t vecSize=unitList.size();
	retVec.reserve(vecSize);
	for(size_t i=0;i<vecSize;i++){
		retVec.push_back(UnitLogData(unitList[i]));
	}
	return retVec;
}
