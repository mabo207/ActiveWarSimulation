#include"ScoreRankingData.h"
#include"FileRead.h"

//----------ScoreRankingData-------------
const std::string ScoreRankingData::scoreRankingTxtPass="SaveData/scoreRanking.txt";

ScoreRankingData::ScoreRankingData()
{
	//ステージ15,各ステージ難易度4つ,各レベル5つのデータが存在しているの時の速度（読み込み：0.96秒 コピー：0.30秒 書き込み：0.02秒）
	FpsMeasuring fps;
	//読み込み(0.30秒)
	fps.RecordTime();
	StringBuilder strBuilder(FileStrRead(scoreRankingTxtPass.c_str()),'\n','{','}',false,true);
	volatile const double t1=fps.GetProcessedTime();
	//各ステージデータの読み込み(0.66秒)
	for(StringBuilder &sb:strBuilder.m_vec){
		sb.Split(',','(',')',true);
		std::string dirName;
		const std::array<const StringBuilder *,StageScoreData::levelCount> initArray={nullptr,nullptr,nullptr,nullptr};
		std::array<const StringBuilder *,StageScoreData::levelCount> pBuilderArray=initArray;
		for(const StringBuilder &ssb:sb.m_vec){
			if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="dir"){
				//ディレクトリデータの読み込み
				dirName=ssb.m_vec[1].GetString();
			} else if(ssb.m_vec.size()>=5 && ssb.m_vec[0].GetString()=="data"){
				//データがどこにあるかを渡す
				pBuilderArray={&ssb.m_vec[1],&ssb.m_vec[2],&ssb.m_vec[3],&ssb.m_vec[4]};
			}
		}
		if(pBuilderArray!=initArray && !dirName.empty()){
			m_stageDataMap.insert(std::make_pair(dirName,StageScoreData::Create(dirName,pBuilderArray)));
		}
	}
	volatile const double totalT=fps.GetProcessedTime();
	volatile const double t2=totalT-t1;
	int a=0;
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

//----------ScoreRankingData::DataCreateException-------------
ScoreRankingData::DataCreateException::DataCreateException()noexcept{}

ScoreRankingData::DataCreateException::~DataCreateException()noexcept{}

const char *ScoreRankingData::DataCreateException::what()const noexcept{
	return "Data Create Failed.";
}

//----------ScoreRankingData::PlayerData-------------
bool ScoreRankingData::PlayerData::operator<(const PlayerData &otherobj)const{
	//スコアで比較
	if(this->score<otherobj.score){
		return true;
	} else if(this->score>otherobj.score){
		return false;
	}
	//dateで比較
	if(this->date<otherobj.date){
		return true;
	} else if(this->date>otherobj.date){
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
	std::string name="";
	int score=-99999;
	std::string date="";
	for(const StringBuilder &sb:infoBuilder.m_vec){
		if(sb.m_vec.size()>=2){
			const std::string str=sb.m_vec[0].GetString();
			if(str=="name"){
				name=sb.m_vec[1].GetString();
			} else if(str=="score"){
				score=std::atoi(sb.m_vec[1].GetString().c_str());
			} else if(str=="date"){
				date=sb.m_vec[1].GetString();
			}
		}
	}
	if(name!="" && score!=-99999 && date!=""){
		//しっかりとデータが存在していればPlayerDataを作成
		return PlayerData(score,name,date);
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
			playerDataVec.insert(PlayerData::Create(sb));
		} catch(std::exception &e){
			//正常にPlayerDataが作られない時は、要素に追加しなければ良い
		}
	}
}

void ScoreRankingData::LevelData::Output(std::ofstream &ofs)const{
	ofs<<'(';
	for(auto it=playerDataVec.begin(),ite=playerDataVec.end();;){
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
	:dirName("")
	,levelArray{LevelData(),LevelData(),LevelData(),LevelData()}
{}

void ScoreRankingData::StageScoreData::Output(std::ofstream &ofs)const{
	ofs<<"(data,";
	for(size_t i=0;i<levelCount;i++){
		ofs<<"((level,"<<i+1<<"),";
		levelArray[i].Output(ofs);
		ofs<<')';
		if(i+1<levelCount){
			//末尾には','をつけない
			ofs<<',';
		}
	}
	ofs<<')';
}

ScoreRankingData::StageScoreData ScoreRankingData::StageScoreData::Create(const std::string &i_dirName,const std::array<const StringBuilder *,levelCount> &infoBuilderPointerArray){
	//分割された文字列を認識
	std::array<LevelData,levelCount> levelArray;
	for(size_t i=0;i<levelCount;i++){
		if(infoBuilderPointerArray[i]->m_vec.size()>=2){
			//ステージレベルを取得
			int stageLevel=-1;
			if(infoBuilderPointerArray[i]->m_vec[0].m_vec.size()>=2
				&& infoBuilderPointerArray[i]->m_vec[0].m_vec[0].GetString()=="level")
			{
				stageLevel=std::atoi(infoBuilderPointerArray[i]->m_vec[0].m_vec[1].GetString().c_str());
			}
			//ステージレベルを取得できた場合のみ、LevelDataを作成
			const int index=stageLevel-1;
			if(index>=0 && index<levelCount){
				levelArray[index]=LevelData(infoBuilderPointerArray[i]->m_vec[1]);
			}
		}
	}
	return StageScoreData(i_dirName,levelArray);
}