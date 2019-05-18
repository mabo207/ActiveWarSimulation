#include"ScoreRankingData.h"
#include"FileRead.h"
#include<fstream>

//----------ScoreRankingData-------------
const std::string ScoreRankingData::scoreRankingTxtPass="SaveData/scoreRanking.txt";

ScoreRankingData::ScoreRankingData()
{
	//�ǂݍ���
	StringBuilder strBuilder(FileStrRead(scoreRankingTxtPass.c_str()),'\n','{','}',false,true);
	//�e�X�e�[�W�f�[�^�̓ǂݍ���
	for(StringBuilder &sb:strBuilder.m_vec){
		sb.Split(',','(',')',true);
		std::string dirName;
		const std::array<const StringBuilder *,4> initArray={nullptr,nullptr,nullptr,nullptr};
		std::array<const StringBuilder *,4> pBuilderArray=initArray;
		for(const StringBuilder &ssb:sb.m_vec){
			if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="dir"){
				//�f�B���N�g���f�[�^�̓ǂݍ���
				dirName=ssb.m_vec[1].GetString();
			} else if(ssb.m_vec.size()>=5 && ssb.m_vec[0].GetString()=="data"){
				//�f�[�^���ǂ��ɂ��邩��n��
				pBuilderArray={&ssb.m_vec[1],&ssb.m_vec[2],&ssb.m_vec[3],&ssb.m_vec[4]};
			}
		}
		if(pBuilderArray!=initArray && !dirName.empty()){
			m_stageDataMap.insert(std::make_pair(dirName,StageScoreData::Create(dirName,pBuilderArray)));
		}
	}
}

ScoreRankingData::~ScoreRankingData(){}

bool ScoreRankingData::Save()const{

	return true;
}

//----------ScoreRankingData::DataCreateException-------------
ScoreRankingData::DataCreateException::DataCreateException()noexcept{}

ScoreRankingData::DataCreateException::~DataCreateException()noexcept{}

const char *ScoreRankingData::DataCreateException::what()const noexcept{
	return "Data Create Failed.";
}

//----------ScoreRankingData::PlayerData-------------
bool ScoreRankingData::PlayerData::operator<(const PlayerData &otherobj)const{
	//�X�R�A�Ŕ�r
	if(this->score<otherobj.score){
		return true;
	} else if(this->score>otherobj.score){
		return false;
	}
	//name�Ŕ�r
	if(this->name<otherobj.name){
		return true;
	} else if(this->name>otherobj.name){
		return false;
	}
	//date�Ŕ�r
	return (this->score<otherobj.score);
}

ScoreRankingData::PlayerData ScoreRankingData::PlayerData::Create(const StringBuilder &infoBuilder){
	//�����񕪊�
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
		//��������ƃf�[�^�����݂��Ă����PlayerData���쐬
		return PlayerData(score,name,date);
	}
	//�f�[�^�쐬���s
	throw DataCreateException();
}

//----------ScoreRankingData::LevelData-------------
ScoreRankingData::LevelData::LevelData(const StringBuilder &infoBuilder){
	//�������ꂽ�������F��
	for(const StringBuilder &sb:infoBuilder.m_vec){
		//�v�f�}��
		try{
			playerDataVec.insert(PlayerData::Create(sb));
		} catch(DataCreateException &exception){
			//�����PlayerData������Ȃ����́A�v�f�ɒǉ����Ȃ���Ηǂ�
		}
	}
}

//----------ScoreRankingData::StageScoreData-------------
ScoreRankingData::StageScoreData ScoreRankingData::StageScoreData::Create(const std::string &i_dirName,const std::array<const StringBuilder *,4> &infoBuilderPointerArray){
	//�������ꂽ�������F��
	std::array<LevelData,4> levelArray;
	for(size_t i=0;i<4;i++){
		if(infoBuilderPointerArray[i]->m_vec.size()>=2){
			//�X�e�[�W���x�����擾
			int stageLevel=-1;
			if(infoBuilderPointerArray[i]->m_vec[0].m_vec.size()>=2
				&& infoBuilderPointerArray[i]->m_vec[0].m_vec[0].GetString()=="level")
			{
				stageLevel=std::atoi(infoBuilderPointerArray[i]->m_vec[0].m_vec[1].GetString().c_str());
			}
			//�X�e�[�W���x�����擾�ł����ꍇ�̂݁ALevelData���쐬
			if(stageLevel>=0 && stageLevel<4){
				levelArray[stageLevel]=LevelData(infoBuilderPointerArray[i]->m_vec[1]);
			}
		}
	}
	return StageScoreData(i_dirName,levelArray);
}