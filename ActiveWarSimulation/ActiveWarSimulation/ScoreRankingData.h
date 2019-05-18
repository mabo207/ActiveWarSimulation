#ifndef DEF_SCORERANKINGDATA_H
#define DEF_SCORERANKINGDATA_H

#include<string>
#include<vector>
#include<array>
#include<map>
#include<set>
#include"ToolsLib.h"

//クリアスコアのランキングを管理するクラス
//ローカルでは10KB~100KBくらいのテキストデータを想定している。
//速度にある程度気を遣う
class ScoreRankingData{
public:
	ScoreRankingData();
	~ScoreRankingData();
	bool Save()const;//現在のメンバ変数を用いて記録する

private:
	struct PlayerData{
		//ランキングに載っている１つのスコアに関するデータ
	public:
		const int score;//スコア値
		const std::string name;//ユーザー名
		const std::string date;//日時(これは数値で持つ必要がない。同一スコア同一ユーザーのデータを区別しやすくするためなため。)
		~PlayerData(){}
		bool operator<(const PlayerData &otherobj)const;
		static PlayerData Create(const StringBuilder &infoBuilder);//文字列から作る

	private:
		//メンバ変数をconstにしているので、読み込み文字列の処理をコンストラクタ内でやりたくない。そのためCreate()関数にそれを移譲するため、コンストラクタをprivateにする。
		PlayerData(const int i_score,const std::string &i_name,const std::string &i_date)
			:score(i_score),name(i_name),date(i_date){}
	};
	struct LevelData{
		//１つのステージレベルに対するスコアデータ
		std::set<PlayerData> playerDataVec;//ランキングに表示されるプレイヤーデータ(挿入を可能にするためにconstにしない)
		LevelData(){}
		explicit LevelData(const StringBuilder &infoBuilder);//メンバ変数をconstにする必用はないので、LevelData内で読み込み文字列の処理をして問題ない
		~LevelData(){}
	};
	struct StageScoreData{
		//ステージが持っているランキングに関するデータ
	public:
		const std::string dirName;//ステージのディレクトリ名
		const std::array<LevelData,4> levelArray;//難易度ごとのスコアデータ
		~StageScoreData(){}
		static StageScoreData Create(const std::string &i_dirName,const std::array<const StringBuilder *,4> &infoBuilderPointerArray);//文字列から作成

	private:
		StageScoreData(const std::string &i_dirName,const std::array<LevelData,4> &i_levelArray)
			:dirName(i_dirName),levelArray(i_levelArray){}
	};

	std::map<std::string,StageScoreData> m_stageDataMap;//全ステージのスコア一覧

	static const std::string scoreRankingTxtPass;//管理されているテキストデータのパス
};

#endif // !DEF_SCORERANKINGDATA_H