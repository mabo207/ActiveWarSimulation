#ifndef DEF_SCORERANKINGDATA_H
#define DEF_SCORERANKINGDATA_H

#include<string>
#include<vector>
#include<array>
#include<map>
#include<set>
#include<exception>
#include"ToolsLib.h"
#include<fstream>

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

	private:
		//�����o�ϐ���const�ɂ��Ă���̂ŁA�ǂݍ��ݕ�����̏������R���X�g���N�^���ł�肽���Ȃ��B���̂���Create()�֐��ɂ�����ڏ����邽�߁A�R���X�g���N�^��private�ɂ���B
		PlayerData(const int i_score,const std::string &i_name,const std::string &i_date)
			:score(i_score),name(i_name),date(i_date){}
	};
	struct LevelData{
		//�P�̃X�e�[�W���x���ɑ΂���X�R�A�f�[�^
		std::set<PlayerData> playerDataVec;//�����L���O�ɕ\�������v���C���[�f�[�^(�}�����\�ɂ��邽�߂�const�ɂ��Ȃ�)
		
		LevelData(){}
		explicit LevelData(const StringBuilder &infoBuilder);//�����o�ϐ���const�ɂ���K�p�͂Ȃ��̂ŁALevelData���œǂݍ��ݕ�����̏��������Ė��Ȃ�
		~LevelData(){}
		void Output(std::ofstream &ofs)const;//�f�[�^�o��
	};
	struct StageScoreData{
		//�X�e�[�W�������Ă��郉���L���O�Ɋւ���f�[�^
	public:
		static const size_t levelCount=4;

		const std::string dirName;//�X�e�[�W�̃f�B���N�g����
		const std::array<LevelData,levelCount> levelArray;//��Փx���Ƃ̃X�R�A�f�[�^

		StageScoreData();//�u�f�B���N�g���̃����L���O�f�[�^���Ȃ��v�Ƃ����ꍇ�ł��v���O�����������Ɠ��삳���������߁B
		~StageScoreData(){}
		void Output(std::ofstream &ofs)const;//�f�[�^�o��
		static StageScoreData Create(const std::string &i_dirName,const std::array<const StringBuilder *,levelCount> &infoBuilderPointerArray);//�����񂩂�쐬

	private:
		StageScoreData(const std::string &i_dirName,const std::array<LevelData,levelCount> &i_levelArray)
			:dirName(i_dirName),levelArray(i_levelArray){}
	};

	ScoreRankingData();
	~ScoreRankingData();
	bool Save()const;//���݂̃����o�ϐ���p���ċL�^����
	const StageScoreData GetStageScoreData(const std::string &dirName)const;

private:
	std::map<std::string,StageScoreData> m_stageDataMap;//�S�X�e�[�W�̃X�R�A�ꗗ

	static const std::string scoreRankingTxtPass;//�Ǘ�����Ă���e�L�X�g�f�[�^�̃p�X
};

#endif // !DEF_SCORERANKINGDATA_H