#include"AnySceneCallable.h"
#include"FileRead.h"

StageInfoReader::StageInfoReader(const std::string &dirName){
	//stageInfo.txt�̐��f�[�^��ǂݍ��݉\�Ȍ`���ɕϊ�
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