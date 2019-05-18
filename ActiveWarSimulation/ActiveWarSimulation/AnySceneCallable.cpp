#include"AnySceneCallable.h"
#include"FileRead.h"

StageInfoReader::StageInfoReader(const std::string &dirName){
	//stageInfo.txtの生データを読み込み可能な形式に変換
	const StringBuilder infoBuilder(FileStrRead(("Stage/"+dirName+"/stageInfo.txt").c_str()),',','(',')',true,true);
	for(const StringBuilder &sb:infoBuilder.GetVector()){
		if(sb.m_vec.size()>=2){
			if(sb.m_vec[0].GetString()=="title"){
				titleName=sb.m_vec[1].GetString();
			} else if(sb.m_vec[0].GetString()=="level"){
				stageLevel=std::atoi(sb.m_vec[1].GetString().c_str());
			}
		}
	}
}

StageInfoReader::~StageInfoReader(){}