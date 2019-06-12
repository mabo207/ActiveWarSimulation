#include"StageInfoReader.h"
#include"FileRead.h"
#include"StringBuilder.h"
#include"FilePath.h"

//-----------------StageInfoReader--------------------
StageInfoReader::StageInfoReader(const std::string &dirName){
	//stageInfo.txtの生データを読み込み可能な形式に変換
	const StringBuilder infoBuilder(FileStrRead((FilePath::stageDir+dirName+"/stageInfo.txt").c_str()),',','(',')');
	for(const StringBuilder &sb:infoBuilder.m_vec){
		if(sb.m_vec.size()>=2){
			if(sb.m_vec[0].GetString()=="title"){
				titleName=sb.m_vec[1].GetString();
			}
		}
	}
}

StageInfoReader::~StageInfoReader(){}