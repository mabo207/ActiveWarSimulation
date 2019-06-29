#include"ScoreRankingData.h"
#include"FileRead.h"
#include"ToolsLib.h"
#include"FilePath.h"
#include<optional>

//----------ScoreRankingData-------------
const std::string ScoreRankingData::scoreRankingTxtPass=FilePath::savedataDir+"scoreRanking.txt";

ScoreRankingData::ScoreRankingData()
{
	//�X�e�[�W15,�e�X�e�[�W��Փx4��,�e���x��5�̃f�[�^�����݂��Ă���̎��̑��x�i�ǂݍ��݁F0.96�b �R�s�[�F0.30�b �������݁F0.02�b�j
//	FpsMeasuring fps;
	//�ǂݍ���(0.30�b��0.26�b)
//	fps.RecordTime();
	StringBuilder strBuilder(FileStrRead(scoreRankingTxtPass.c_str()),'\n','{','}');
//	volatile const double t1=fps.GetProcessedTime();
	//�e�X�e�[�W�f�[�^�̓ǂݍ���(0.66�b��0.28�b)
	for(StringBuilder &sb:strBuilder.m_vec){
		sb.Split(',','(',')');
		std::string dirName;
		const StringBuilder *pBuilder=nullptr;
		for(const StringBuilder &ssb:sb.m_vec){
			if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="dir"){
				//�f�B���N�g���f�[�^�̓ǂݍ���
				dirName=ssb.m_vec[1].GetString();
			} else if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="data"){
				//�f�[�^���ǂ��ɂ��邩��n��
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
	//�S�ď��������郂�[�h�Ńt�@�C�����J��
	std::ofstream ofs(scoreRankingTxtPass.c_str(),std::ios_base::trunc);
	if(!ofs){
		return false;
	}
	//��������
	for(const std::pair<std::string,StageScoreData> &pair:m_stageDataMap){
		ofs<<"{(dir,"<<pair.first<<"),";
		pair.second.Output(ofs);
		ofs<<'}'<<std::endl;
	}
	//�I������
	ofs.close();
	return true;
}

const ScoreRankingData::StageScoreData ScoreRankingData::GetStageScoreData(const std::string &dirName)const{
	//dirName�ɊY������v�f���Ȃ���΁A�f�t�H���g�l(�����L���O�f�[�^0��)���Ԃ����
	//return m_stageDataMap[dirName];//const map��[]���g���Ȃ��Ƃ����d�l������B�N�\�B
	//return m_stageDataMap.at(dirName);//�Y�����Ȃ��v�f�������out_of_range��O����ԁB����͍���B
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
		//inputData�Ɠ����X�e�[�W�̃����L���O�f�[�^�����݂���ꍇ
		const auto itLevel=itStage->second.levelMap.find(level);
		if(itLevel!=itStage->second.levelMap.end()){
			//inputData�Ɠ������x���̃����L���O�f�[�^�����݂���ꍇ
			itLevel->second.playerDataSet.insert(inputData);
		} else{
			//�������x���̃����L���O�f�[�^�����݂��Ȃ��ꍇ�́A�쐬����
			LevelData insertData;
			insertData.playerDataSet.insert(inputData);
			itStage->second.levelMap.insert(std::make_pair(level,insertData));
		}
	} else{
		//�����X�e�[�W�̃����L���O�f�[�^�����݂��Ȃ��ꍇ�A�쐬����
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
	//�X�R�A�Ŕ�r(�X�R�A���傫���قǐ�ɗ��ė~����)
	if(this->score>otherobj.score){
		return true;
	} else if(this->score<otherobj.score){
		return false;
	}
	//date�Ŕ�r(date����̓����قǐ�ɗ��ė~����)
	if(this->date>otherobj.date){
		return true;
	} else if(this->date<otherobj.date){
		return false;
	}
	//name�Ŕ�r
	return (this->name<otherobj.name);
}

void ScoreRankingData::PlayerData::Output(std::ofstream &ofs)const{
	ofs<<"((name,"<<name<<"),(score,"<<score<<"),(date,"<<date<<"))";
}

ScoreRankingData::PlayerData ScoreRankingData::PlayerData::Create(const StringBuilder &infoBuilder){
	//�����񕪊�
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
			//�s���l�������Ă����ꍇ
		} catch(const std::out_of_range &){
			//�͈͊O�̒l�������Ă����ꍇ
		}
	}
	if(score && name && date){
		//��������ƃf�[�^�����݂��Ă����PlayerData���쐬
		return PlayerData(score.value(),name.value(),date.value());
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
			playerDataSet.insert(PlayerData::Create(sb));
		} catch(std::exception &){
			//�����PlayerData������Ȃ����́A�v�f�ɒǉ����Ȃ���Ηǂ�
		}
	}
}

void ScoreRankingData::LevelData::Output(std::ofstream &ofs)const{
	ofs<<'(';
	for(auto it=playerDataSet.begin(),ite=playerDataSet.end();;){
		it->Output(ofs);
		//������','����ꂽ���Ȃ��̂ŁA��肭�ǂ�����������
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
			//�����ɂ�','�����Ȃ�
			ofs<<',';
		}
	}
	ofs<<')';
}

ScoreRankingData::StageScoreData ScoreRankingData::StageScoreData::Create(const StringBuilder &infoBuilder){
	//�������ꂽ�������F��
	std::map<StageLevel,LevelData> levelMap;
	for(const StringBuilder &sb:infoBuilder.m_vec){
		if(sb.m_vec.size()>=2){
			//�X�e�[�W���x�����擾
			if(sb.m_vec[0].m_vec.size()>=2 && sb.m_vec[0].m_vec[0].GetString()=="level"){
				try{
					//StageLevel�𕶎���ŏ���������ꍇ�Aout_of_range��O�𓊂���\��������B
					StageLevel stageLevel=StageLevel::CreateFromString(sb.m_vec[0].m_vec[1].GetString());
					//�X�e�[�W���x�����擾�ł����ꍇ�̂݁ALevelData���쐬
					levelMap.insert(std::make_pair(stageLevel,LevelData(sb.m_vec[1])));
				} catch(std::out_of_range &){
					//���ɉ������Ȃ�
				}
			}
		}
	}
	return StageScoreData(levelMap);
}