#include"StageInfoReader.h"
#include"FileRead.h"
#include"StringBuilder.h"
#include"FilePath.h"

//-----------------StageInfoReader--------------------
StageInfoReader::StageInfoReader(const std::string &dirName){
	//stageInfo.txtの生データを読み込み可能な形式に変換
	const StringBuilder infoBuilder(FileStrRead((FilePath::stageDir+dirName+"/stageInfo.txt").c_str()),',','(',')');
	for(const StringBuilder &sb:infoBuilder.m_vec){
		const size_t vecSize=sb.m_vec.size();
		if(vecSize>=2 && sb.m_vec[0].GetString()=="title"){
			titleName=sb.m_vec[1].GetString();
		} else if(vecSize>=3 && sb.m_vec[0].GetString()=="pos"){
			float x=(float)std::atof(sb.m_vec[1].GetString().c_str());
			float y=(float)std::atof(sb.m_vec[2].GetString().c_str());
			pos=Vector2D(x,y);
		}
	}
}

StageInfoReader::~StageInfoReader(){}