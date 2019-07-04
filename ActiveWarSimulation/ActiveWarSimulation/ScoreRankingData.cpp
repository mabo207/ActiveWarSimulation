#include"ScoreRankingData.h"
#include"FileRead.h"
#include"ToolsLib.h"
#include"FilePath.h"
#include<optional>

//----------ScoreRankingData-------------
const std::string ScoreRankingData::scoreRankingTxtPass=FilePath::savedataDir+"scoreRanking.txt";

ScoreRankingData::ScoreRankingData()
{
	//ステージ15,各ステージ難易度4つ,各レベル5つのデータが存在しているの時の速度（読み込み：0.96秒 コピー：0.30秒 書き込み：0.02秒）
//	FpsMeasuring fps;
	//読み込み(0.30秒→0.26秒)
//	fps.RecordTime();
	StringBuilder strBuilder(FileStrRead(scoreRankingTxtPass.c_str()),'\n','{','}');
//	volatile const double t1=fps.GetProcessedTime();
	//各ステージデータの読み込み(0.66秒→0.28秒)
	for(StringBuilder &sb:strBuilder.m_vec){
		sb.Split(',','(',')');
		std::string dirName;
		const StringBuilder *pBuilder=nullptr;
		for(const StringBuilder &ssb:sb.m_vec){
			if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="dir"){
				//ディレクトリデータの読み込み
				dirName=ssb.m_vec[1].GetString();
			} else if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="data"){
				//データがどこにあるかを渡す
				pBuilder=&ssb;
			}
		}
		if(!dirName.empty() && pBuilder!=nullptr){
			m_stageDataMap.insert(std::make_pair(dirName,StageScoreData::Create(*pBuilder)));
		}
	}
//	volatile const double totalT=fps.GetProcessedTime();
//	volatile const double t2=totalT-t1;
//	int a=0;
}

ScoreRankingData::~ScoreRankingData(){}

bool ScoreRankingData::Save()const{
	//全て書き換えるモードでファイルを開く
	std::ofstream ofs(scoreRankingTxtPass.c_str(),std::ios_base::trunc);
	if(!ofs){
		return false;
	}
	//書き込み
	for(const std::pair<std::string,StageScoreData> &pair:m_stageDataMap){
		ofs<<"{(dir,"<<pair.first<<"),";
		pair.second.Output(ofs);
		ofs<<'}'<<std::endl;
	}
	//終了処理
	ofs.close();
	return true;
}

const ScoreRankingData::StageScoreData ScoreRankingData::GetStageScoreData(const std::string &dirName)const{
	//dirNameに該当する要素がなければ、デフォルト値(ランキングデータ0件)が返される
	//return m_stageDataMap[dirName];//const mapは[]を使えないという仕様がある。クソ。
	//return m_stageDataMap.at(dirName);//該当しない要素があるとout_of_range例外が飛ぶ。それは困る。
	const auto it=m_stageDataMap.find(dirName);
	if(it!=m_stageDataMap.end()){
		return it->second;
	} else{
		return StageScoreData();
	}
}

bool ScoreRankingData::InputData(PlayerData &inputData,const std::string &dirName,const StageLevel level){
	const auto itStage=m_stageDataMap.find(dirName);
	if(itStage!=m_stageDataMap.end()){
		//inputDataと同じステージのランキングデータが存在する場合
		const auto itLevel=itStage->second.levelMap.find(level);
		if(itLevel!=itStage->second.levelMap.end()){
			//inputDataと同じレベルのランキングデータが存在する場合
			itLevel->second.playerDataSet.insert(inputData);
		} else{
			//同じレベルのランキングデータが存在しない場合は、作成する
			LevelData insertData;
			insertData.playerDataSet.insert(inputData);
			itStage->second.levelMap.insert(std::make_pair(level,insertData));
		}
	} else{
		//同じステージのランキングデータが存在しない場合、作成する
		LevelData insertLevelData;
		insertLevelData.playerDataSet.insert(inputData);
		StageScoreData insertScoreData;
		insertScoreData.levelMap.insert(std::make_pair(level,insertLevelData));
		m_stageDataMap.insert(std::make_pair(dirName,insertScoreData));
	}
	return false;
}

//----------ScoreRankingData::DataCreateException-------------
ScoreRankingData::DataCreateException::DataCreateException()noexcept{}

ScoreRankingData::DataCreateException::~DataCreateException()noexcept{}

const char *ScoreRankingData::DataCreateException::what()const noexcept{
	return "Data Create Failed.";
}

//----------ScoreRankingData::PlayerData-------------
bool ScoreRankingData::PlayerData::operator<(const PlayerData &otherobj)const{
	//スコアで比較(スコアが大きいほど先に来て欲しい)
	if(this->score>otherobj.score){
		return true;
	} else if(this->score<otherobj.score){
		return false;
	}
	//dateで比較(dateが後の日時ほど先に来て欲しい)
	if(this->date>otherobj.date){
		return true;
	} else if(this->date<otherobj.date){
		return false;
	}
	//nameで比較
	return (this->name<otherobj.name);
}

void ScoreRankingData::PlayerData::Output(std::ofstream &ofs)const{
	ofs<<"((name,"<<name<<"),(score,"<<score<<"),(date,"<<date<<"))";
}

ScoreRankingData::PlayerData ScoreRankingData::PlayerData::Create(const StringBuilder &infoBuilder){
	//文字列分割
	std::optional<int> score;
	std::optional<std::string> name;
	std::optional<__time64_t> date;
	for(const StringBuilder &sb:infoBuilder.m_vec){
		try{
			if(sb.m_vec.size()>=2){
				const std::string str=sb.m_vec[0].GetString();
				if(str=="name"){
					name=sb.m_vec[1].GetString();
				} else if(str=="score"){
					score=std::stoi(sb.m_vec[1].GetString().c_str());
				} else if(str=="date"){
					date=std::stoll(sb.m_vec[1].GetString().c_str());
				}
			}
		} catch(const std::invalid_argument &){
			//不正値が入っていた場合
		} catch(const std::out_of_range &){
			//範囲外の値が入っていた場合
		}
	}
	if(score && name && date){
		//しっかりとデータが存在していればPlayerDataを作成
		return PlayerData(score.value(),name.value(),date.value());
	}
	//データ作成失敗
	throw DataCreateException();
}

//----------ScoreRankingData::LevelData-------------
ScoreRankingData::LevelData::LevelData(const StringBuilder &infoBuilder){
	//分割された文字列を認識
	for(const StringBuilder &sb:infoBuilder.m_vec){
		//要素挿入
		try{
			playerDataSet.insert(PlayerData::Create(sb));
		} catch(std::exception &){
			//正常にPlayerDataが作られない時は、要素に追加しなければ良い
		}
	}
}

void ScoreRankingData::LevelData::Output(std::ofstream &ofs)const{
	ofs<<'(';
	for(auto it=playerDataSet.begin(),ite=playerDataSet.end();;){
		it->Output(ofs);
		//末尾に','を入れたくないので、回りくどい処理をする
		it++;
		if(it!=ite){
			ofs<<',';
		} else{
			break;
		}		
	}
	ofs<<')';
}

//----------ScoreRankingData::StageScoreData-------------
ScoreRankingData::StageScoreData::StageScoreData()
	:levelMap{}
{}

void ScoreRankingData::StageScoreData::Output(std::ofstream &ofs)const{
	ofs<<"(data,";
	for(std::map<StageLevel,LevelData>::const_iterator it=levelMap.begin(),ite=levelMap.end();it!=ite;){
		ofs<<"((level,"<<it->first.GetString()<<"),";
		it->second.Output(ofs);
		ofs<<')';
		it++;
		if(it!=ite){
			//末尾には','をつけない
			ofs<<',';
		}
	}
	ofs<<')';
}

ScoreRankingData::StageScoreData ScoreRankingData::StageScoreData::Create(const StringBuilder &infoBuilder){
	//分割された文字列を認識
	std::map<StageLevel,LevelData> levelMap;
	for(const StringBuilder &sb:infoBuilder.m_vec){
		if(sb.m_vec.size()>=2){
			//ステージレベルを取得
			if(sb.m_vec[0].m_vec.size()>=2 && sb.m_vec[0].m_vec[0].GetString()=="level"){
				try{
					//StageLevelを文字列で初期化する場合、out_of_range例外を投げる可能性がある。
					StageLevel stageLevel=StageLevel::CreateFromString(sb.m_vec[0].m_vec[1].GetString());
					//ステージレベルを取得できた場合のみ、LevelDataを作成
					levelMap.insert(std::make_pair(stageLevel,LevelData(sb.m_vec[1])));
				} catch(std::out_of_range &){
					//特に何もしない
				}
			}
		}
	}
	return StageScoreData(levelMap);
}