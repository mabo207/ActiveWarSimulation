#include"StageLevel.h"

//-----------------StageLevel------------------
const std::unordered_map<std::string,StageLevel::Kind> StageLevel::kindStringMap={
	std::make_pair("1",StageLevel::e_easy)
	,std::make_pair("2",StageLevel::e_normal)
	,std::make_pair("3",StageLevel::e_hard)
	,std::make_pair("4",StageLevel::e_lunatic)
	/*
		std::make_pair("easy",StageLevel::e_easy)
		,std::make_pair("normal",StageLevel::e_normal)
		,std::make_pair("hard",StageLevel::e_hard)
		,std::make_pair("lunatic",StageLevel::e_lunatic)
	//*/
};

std::string StageLevel::GetString()const{
	for(const auto &pair:kindStringMap){
		if(pair.second==m_kind){
			return pair.first;
		}
	}
	//—áŠOˆ—
	return "";
}

size_t StageLevel::GetIndex()const{
	if(m_kind==StageLevel::e_easy){
		return 0;
	} else if(m_kind==StageLevel::e_normal){
		return 1;
	} else if(m_kind==StageLevel::e_hard){
		return 2;
	} else if(m_kind==StageLevel::e_lunatic){
		return 3;
	}
	return levelCount;
}