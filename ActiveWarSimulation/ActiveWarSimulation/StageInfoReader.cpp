#include"StageInfoReader.h"
#include"FileRead.h"
#include"StringBuilder.h"

//-----------------StageInfoReader--------------------
StageInfoReader::StageInfoReader(const std::string &dirName){
	//stageInfo.txt�̐��f�[�^��ǂݍ��݉\�Ȍ`���ɕϊ�
	const StringBuilder infoBuilder(FileStrRead(("Stage/"+dirName+"/stageInfo.txt").c_str()),',','(',')');
	for(const StringBuilder &sb:infoBuilder.m_vec){
		if(sb.m_vec.size()>=2){
			if(sb.m_vec[0].GetString()=="title"){
				titleName=sb.m_vec[1].GetString();
			} else if(sb.m_vec[0].GetString()=="level"){
				stageLevel=StageLevel(sb.m_vec[1].GetString().c_str()).m_kind;
			}
		}
	}
}

StageInfoReader::~StageInfoReader(){}