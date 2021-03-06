#ifndef DEF_STAGELEVEL_H
#define DEF_STAGELEVEL_H

#include<array>
#include<unordered_map>
#include<string>

//ステージレベルについての情報
struct StageLevel{
	//StageLevelを要素がStageLevel::Kindなenum classっぽく使えるように工夫する
public:
	enum Kind{
		//要素追加時、levelCountやlevelArray変数も変えておくこと
		e_easy
		,e_normal
		,e_hard
		,e_lunatic
	};

	StageLevel()noexcept;
	StageLevel(Kind kind)noexcept;
	bool operator==(const StageLevel &otherobj)const;
	bool operator<(const StageLevel &otherobj)const;
	std::string GetString()const;
	size_t GetIndex()const;
	StageLevel Shift(int shift)const;//levelArrayに沿って要素が並んでいる時、indexだけずらした時の要素が欲しい！
	static StageLevel CreateFromString(const std::string &str);

	static const size_t levelCount=e_lunatic+1;//レベルの数
	static const std::array<StageLevel,levelCount> levelArray;//レベル一覧

private:
	Kind m_kind;
	//種類と文字列の対応表
	static const std::unordered_map<std::string,Kind> kindStringMap;
};

#endif // !DEF_STAGELEVEL_H