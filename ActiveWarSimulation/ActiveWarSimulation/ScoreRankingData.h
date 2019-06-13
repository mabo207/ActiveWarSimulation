#ifndef DEF_SCORERANKINGDATA_H
#define DEF_SCORERANKINGDATA_H

#include<string>
#include<vector>
#include<map>
#include<set>
#include<exception>
#include<fstream>
#include"StringBuilder.h"
#include"StageLevel.h"

//�N���A�X�R�A�̃����L���O���Ǘ�����N���X
//���[�J���ł�10KB~100KB���炢�̃e�L�X�g�f�[�^��z�肵�Ă���B
//���x�ɂ�����x�C������
class ScoreRankingData{
public:
	class DataCreateException:public std::exception{
	public:
		//PlayerData,LevelData,StageScoreData�̍쐬�Ɏ��s�������ɏo����O
		DataCreateException()noexcept;
		virtual ~DataCreateException()noexcept;
		const char *what()const noexcept;
	};
	struct PlayerData{
		//�����L���O�ɍڂ��Ă���P�̃X�R�A�Ɋւ���f�[�^
	public:
		const int score;//�X�R�A�l
		const std::string name;//���[�U�[��
		const std::string date;//����(����͐��l�Ŏ��K�v���Ȃ��B����X�R�A���ꃆ�[�U�[�̃f�[�^����ʂ��₷�����邽�߂Ȃ��߁B)

		~PlayerData(){}
		bool operator<(const PlayerData &otherobj)const;
		void Output(std::ofstream &ofs)const;//�f�[�^���o�͂���
		static PlayerData Create(const StringBuilder &infoBuilder);//�����񂩂���
		//�����o�ϐ���const�ɂ��Ă���̂ŁA�ǂݍ��ݕ�����̏������R���X�g���N�^���ł�肽���Ȃ��B���̂���Create()�֐��ɂ�����ڏ�����B
		//�������APlayerData������đ}���A�Ƃ������Ƃ��s���̂ŁApublic�ɂ���B
		PlayerData(const int i_score,const std::string &i_name,const std::string &i_date)
			:score(i_score),name(i_name),date(i_date){}
	};
	struct LevelData{
		//�P�̃X�e�[�W���x���ɑ΂���X�R�A�f�[�^
		std::set<PlayerData> playerDataVec;//�����L���O�ɕ\�������v���C���[�f�[�^(�}�����\�ɂ��邽�߂�const�ɂ��Ȃ�)
		
		LevelData(){}
		LevelData(const StringBuilder &infoBuilder);//�����o�ϐ���const�ɂ���K�p�͂Ȃ��̂ŁALevelData���œǂݍ��ݕ�����̏��������Ė��Ȃ�
		~LevelData(){}
		void Output(std::ofstream &ofs)const;//�f�[�^�o��
	};
	struct StageScoreData{
		//�X�e�[�W�������Ă��郉���L���O�Ɋւ���f�[�^
	public:
		std::map<StageLevel,LevelData> levelArray;//��Փx���Ƃ̃X�R�A�f�[�^(�}���̉\��������̂ŁAconst�ɂł��Ȃ�)

		StageScoreData();//�u�f�B���N�g���̃����L���O�f�[�^���Ȃ��v�Ƃ����ꍇ�ł��v���O�����������Ɠ��삳���������߁B
		~StageScoreData(){}
		void Output(std::ofstream &ofs)const;//�f�[�^�o��
		static StageScoreData Create(const StringBuilder &infoBuilder);//�����񂩂�쐬

	private:
		StageScoreData(const std::map<StageLevel,LevelData> &i_levelArray)
			:levelArray(i_levelArray){}
	};

	ScoreRankingData();
	~ScoreRankingData();
	bool Save()const;//���݂̃����o�ϐ���p���ċL�^����
	const StageScoreData GetStageScoreData(const std::string &dirName)const;
	bool InputData(PlayerData &inputData,const std::string &dirName,const StageLevel level);

private:
	std::map<std::string,StageScoreData> m_stageDataMap;//�S�X�e�[�W�̃X�R�A�ꗗ

	static const std::string scoreRankingTxtPass;//�Ǘ�����Ă���e�L�X�g�f�[�^�̃p�X
};

#endif // !DEF_SCORERANKINGDATA_H