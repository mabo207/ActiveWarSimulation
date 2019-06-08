#ifndef DEF_STAGELEVEL_H
#define DEF_STAGELEVEL_H

#include<unordered_map>
#include<string>

//ステージレベルについての情報
struct StageLevel{
	//StageLevelを要素がStageLevel::Kindなenum classっぽく使えるように工夫する
public:
	enum Kind{
		e_easy
		,e_normal
		,e_hard
		,e_lunatic
	};

	StageLevel():m_kind(e_easy){}
	StageLevel(Kind kind):m_kind(kind){}
	explicit StageLevel(const std::string &str):StageLevel(kindStringMap.at(str)){}
	bool operator==(const Kind kind)const{
		return m_kind==kind;
	}
	std::string GetString()const{
		for(const auto &pair:kindStringMap){
			if(pair.second==m_kind){
				return pair.first;
			}
		}
		//例外処理
		return "";
	}

	Kind m_kind;

private:
	//種類と文字列の対応表
	static const std::unordered_map<std::string,Kind> kindStringMap;
};



#endif // !DEF_STAGELEVEL_H