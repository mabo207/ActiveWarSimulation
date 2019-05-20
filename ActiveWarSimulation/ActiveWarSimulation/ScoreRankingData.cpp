#include"ScoreRankingData.h"
#include"FileRead.h"

//----------ScoreRankingData-------------
const std::string ScoreRankingData::scoreRankingTxtPass="SaveData/scoreRanking.txt";

ScoreRankingData::ScoreRankingData()
{
	//�X�e�[�W15,�e�X�e�[�W��Փx4��,�e���x��5�̃f�[�^�����݂��Ă���̎��̑��x�i�ǂݍ��݁F0.96�b �R�s�[�F0.30�b �������݁F0.02�b�j
	FpsMeasuring fps;
	//�ǂݍ���(0.30�b)
	fps.RecordTime();
	StringBuilder strBuilder(FileStrRead(scoreRankingTxtPass.c_str()),'\n','{','}',false,true);
	volatile const double t1=fps.GetProcessedTime();
	//�e�X�e�[�W�f�[�^�̓ǂݍ���(0.66�b)
	for(StringBuilder &sb:strBuilder.m_vec){
		sb.Split(',','(',')',true);
		std::string dirName;
		const std::array<const StringBuilder *,StageScoreData::levelCount> initArray={nullptr,nullptr,nullptr,nullptr};
		std::array<const StringBuilder *,StageScoreData::levelCount> pBuilderArray=initArray;
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
	volatile const double totalT=fps.GetProcessedTime();
	volatile const double t2=totalT-t1;
	int a=0;
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
	//date�Ŕ�r
	if(this->date<otherobj.date){
		return true;
	} else if(this->date>otherobj.date){
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
		} catch(std::exception &e){
			//�����PlayerData������Ȃ����́A�v�f�ɒǉ����Ȃ���Ηǂ�
		}
	}
}

void ScoreRankingData::LevelData::Output(std::ofstream &ofs)const{
	ofs<<'(';
	for(auto it=playerDataVec.begin(),ite=playerDataVec.end();;){
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
			//�����ɂ�','�����Ȃ�
			ofs<<',';
		}
	}
	ofs<<')';
}

ScoreRankingData::StageScoreData ScoreRankingData::StageScoreData::Create(const std::string &i_dirName,const std::array<const StringBuilder *,levelCount> &infoBuilderPointerArray){
	//�������ꂽ�������F��
	std::array<LevelData,levelCount> levelArray;
	for(size_t i=0;i<levelCount;i++){
		if(infoBuilderPointerArray[i]->m_vec.size()>=2){
			//�X�e�[�W���x�����擾
			int stageLevel=-1;
			if(infoBuilderPointerArray[i]->m_vec[0].m_vec.size()>=2
				&& infoBuilderPointerArray[i]->m_vec[0].m_vec[0].GetString()=="level")
			{
				stageLevel=std::atoi(infoBuilderPointerArray[i]->m_vec[0].m_vec[1].GetString().c_str());
			}
			//�X�e�[�W���x�����擾�ł����ꍇ�̂݁ALevelData���쐬
			const int index=stageLevel-1;
			if(index>=0 && index<levelCount){
				levelArray[index]=LevelData(infoBuilderPointerArray[i]->m_vec[1]);
			}
		}
	}
	return StageScoreData(i_dirName,levelArray);
}