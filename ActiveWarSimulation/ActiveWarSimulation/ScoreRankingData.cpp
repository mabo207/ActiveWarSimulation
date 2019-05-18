#include"ScoreRankingData.h"
#include"FileRead.h"
#include"ToolsLib.h"

//----------ScoreRankingData-------------
const std::string ScoreRankingData::scoreRankingTxtPass="SaveData/scoreRanking.txt";

ScoreRankingData::ScoreRankingData()
{
	const StringBuilder strBuilder(FileStrRead(scoreRankingTxtPass.c_str()),'\n','{','}',false,true);
	for(const StringBuilder &sb:strBuilder.m_vec){

	}
}


//----------ScoreRankingData::PlayerData-------------



//----------ScoreRankingData::LevelData-------------



//----------ScoreRankingData::StageScoreData-------------
