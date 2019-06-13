#ifndef DEF_SCORERANKINGDATA_H
#define DEF_SCORERANKINGDATA_H

#include<string>
#include<vector>
#include<map>
#include<set>
#include<exception>
#include<fstream>
#include"StringBuilder.h"
#include"StageLevel.h"

//クリアスコアのランキングを管理するクラス
//ローカルでは10KB~100KBくらいのテキストデータを想定している。
//速度にある程度気を遣う
class ScoreRankingData{
public:
	class DataCreateException:public std::exception{
	public:
		//PlayerData,LevelData,StageScoreDataの作成に失敗した時に出す例外
		DataCreateException()noexcept;
		virtual ~DataCreateException()noexcept;
		const char *what()const noexcept;
	};
	struct PlayerData{
		//ランキングに載っている１つのスコアに関するデータ
	public:
		const int score;//スコア値
		const std::string name;//ユーザー名
		const std::string date;//日時(これは数値で持つ必要がない。同一スコア同一ユーザーのデータを区別しやすくするためなため。)

		~PlayerData(){}
		bool operator<(const PlayerData &otherobj)const;
		void Output(std::ofstream &ofs)const;//データを出力する
		static PlayerData Create(const StringBuilder &infoBuilder);//文字列から作る
		//メンバ変数をconstにしているので、読み込み文字列の処理をコンストラクタ内でやりたくない。そのためCreate()関数にそれを移譲する。
		//ただし、PlayerDataを作って挿入、ということを行うので、publicにする。
		PlayerData(const int i_score,const std::string &i_name,const std::string &i_date)
			:score(i_score),name(i_name),date(i_date){}
	};
	struct LevelData{
		//１つのステージレベルに対するスコアデータ
		std::set<PlayerData> playerDataVec;//ランキングに表示されるプレイヤーデータ(挿入を可能にするためにconstにしない)
		
		LevelData(){}
		LevelData(const StringBuilder &infoBuilder);//メンバ変数をconstにする必用はないので、LevelData内で読み込み文字列の処理をして問題ない
		~LevelData(){}
		void Output(std::ofstream &ofs)const;//データ出力
	};
	struct StageScoreData{
		//ステージが持っているランキングに関するデータ
	public:
		std::map<StageLevel,LevelData> levelArray;//難易度ごとのスコアデータ(挿入の可能性があるので、constにできない)

		StageScoreData();//「ディレクトリのランキングデータがない」という場合でもプログラムをちゃんと動作させたいため。
		~StageScoreData(){}
		void Output(std::ofstream &ofs)const;//データ出力
		static StageScoreData Create(const StringBuilder &infoBuilder);//文字列から作成

	private:
		StageScoreData(const std::map<StageLevel,LevelData> &i_levelArray)
			:levelArray(i_levelArray){}
	};

	ScoreRankingData();
	~ScoreRankingData();
	bool Save()const;//現在のメンバ変数を用いて記録する
	const StageScoreData GetStageScoreData(const std::string &dirName)const;
	bool InputData(PlayerData &inputData,const std::string &dirName,const StageLevel level);

private:
	std::map<std::string,StageScoreData> m_stageDataMap;//全ステージのスコア一覧

	static const std::string scoreRankingTxtPass;//管理されているテキストデータのパス
};

#endif // !DEF_SCORERANKINGDATA_H