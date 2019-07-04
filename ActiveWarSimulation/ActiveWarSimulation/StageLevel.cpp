#include"StageLevel.h"

//-----------------StageLevel------------------
const std::array<StageLevel,StageLevel::levelCount> StageLevel::levelArray={
	StageLevel::e_easy
	,StageLevel::e_normal
	,StageLevel::e_hard
	,StageLevel::e_lunatic
};

StageLevel::StageLevel()noexcept
	:StageLevel(e_easy)
{}

StageLevel::StageLevel(StageLevel::Kind kind)noexcept
	:m_kind(kind)
{}

bool StageLevel::operator==(const StageLevel &otherobj)const{
	return this->m_kind==otherobj.m_kind;
}

bool StageLevel::operator<(const StageLevel &otherobj)const{
	return (this->m_kind<otherobj.m_kind);
}

const std::unordered_map<std::string,StageLevel::Kind> StageLevel::kindStringMap={
	std::make_pair("easy",StageLevel::e_easy)
	,std::make_pair("normal",StageLevel::e_normal)
	,std::make_pair("hard",StageLevel::e_hard)
	,std::make_pair("lunatic",StageLevel::e_lunatic)
};

std::string StageLevel::GetString()const{
	for(const auto &pair:kindStringMap){
		if(pair.second==m_kind){
			return pair.first;
		}
	}
	//例外処理
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

StageLevel StageLevel::Shift(int shift)const{
	//この値がlevelArrayのどこにあるかを確認
	size_t index=0;
	for(;index<levelCount;index++){
		if(levelArray[index]==*this){
			break;
		}
	}
	if(index==levelCount){
		//例外的な値が存在した場合は元と同じ値を返す
		return *this;
	}
	//負の値の剰余は言語やコンパイラによって違う可能性があるので注意する。
	size_t gap;
	if(shift>=0){
		gap=shift%levelCount;
	} else{
		gap=levelCount-((-shift)%levelCount);//減算をしているが正の値になる
	}
	index=(index+gap)%levelCount;
	return levelArray[index];
}

StageLevel StageLevel::CreateFromString(const std::string &str){
	return StageLevel(kindStringMap.at(str));
}