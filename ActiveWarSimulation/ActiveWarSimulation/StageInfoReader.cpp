#include"StageInfoReader.h"
#include"FileRead.h"
#include"StringBuilder.h"
#include"FilePath.h"

//-----------------StageInfoReader--------------------
StageInfoReader::StageInfoReader(const std::string &dirName){
	//stageInfo.txt�̐��f�[�^��ǂݍ��݉\�Ȍ`���ɕϊ�
	const StringBuilder infoBuilder(FileStrRead((FilePath::stageDir+dirName+"/stageInfo.txt").c_str()),',','(',')');
	for(const StringBuilder &sb:infoBuilder.m_vec){
		const size_t vecSize=sb.m_vec.size();
		if(vecSize>=2 && sb.m_vec[0].GetString()=="title"){
			titleName=sb.m_vec[1].GetString();
		} else if(vecSize>=3 && sb.m_vec[0].GetString()=="pos"){
			try{
				float x=(float)std::stof(sb.m_vec[1].GetString().c_str());
				float y=(float)std::stof(sb.m_vec[2].GetString().c_str());
				pos=Vector2D(x,y);
			} catch(const std::invalid_argument &){
				//���l�łȂ�
			} catch(const std::out_of_range &){
				//�͈͊O
			}
		}
	}
}

StageInfoReader::~StageInfoReader(){}