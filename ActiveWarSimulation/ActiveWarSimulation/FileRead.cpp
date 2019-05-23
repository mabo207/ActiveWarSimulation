#include<iostream>
#include<fstream>
#include<cassert>
#include"FileRead.h"


//�t�@�C����ǂݍ���std::vector<char>�^�ɂ��ĕԂ�
std::vector<char> FileRead(const char *DatabaseName){
	//�t�@�C�����J��
	std::ifstream ifs(DatabaseName);
	if(!ifs){
		assert(false);
		return std::vector<char>();
	}
	//������ǂݍ���
	std::vector<char> OriginalStr;
	while(true){
		char word=ifs.get();
		if(word==EOF){//�ǂݎ�������̂�EOF�ɂȂ�܂Ń��[�v����
			break;
		}
		OriginalStr.push_back(word);
	}

	//�t�@�C�������
	ifs.close();

	return OriginalStr;
}

//�t�@�C����ǂݍ��݁A���s�E�J���}�ŋ�؂�ꂽ��������ʂ��ĕԂ�(�����`��)
std::vector<std::vector<std::string>> CommaCutRead(const char *DatabaseName){
	//�t�@�C���̓ǂݍ���
	std::vector<char> OriginalData=FileRead(DatabaseName);
	//���s���Ƀf�[�^�𕪂��āA�P�s���ɂ��ăJ���}��؂�ŕ������ǂݎ��
	std::vector<std::vector<std::string>> CutData;//������Ԃ�
	std::vector<std::string> LineData;//�P�s���̃f�[�^
	std::string Str="";//LineData�ɓ���镶����
	for(unsigned int i=0;i<OriginalData.size();i++){
		switch(OriginalData[i]){
		case('\n')://���s�����̎�
			if(Str!=""){
				LineData.push_back(Str);//Str�ɉ��������Ă����LineData�ɉ�����
			}
			CutData.push_back(LineData);//CutData�ɑ}��
			//������
			LineData.clear();
			Str.clear();
			break;
		case(',')://�J���}�̎�
			LineData.push_back(Str);//LineData�ɑ}��
			//������
			Str.clear();
			break;
		default://����ȊO�̎�
			Str=Str+OriginalData[i];//������ɕ�����ǉ�
			break;
		}
	}
	return CutData;
}

//�t�@�C����ǂݍ���std::string�^�ɂ��ĕԂ�
std::string FileStrRead(const char *DatabaseName){
	//�t�@�C���̓ǂݍ���
	std::vector<char> OriginalData=FileRead(DatabaseName);
	//std::string�^�ɒ���
	std::string Str;
	for(unsigned int i=0;i<OriginalData.size();i++){
		Str=Str+OriginalData[i];
	}
	return Str;
}

//CSV�t�H���_��ǂݎ����std::vector<std::vector<int>>�^�ɂ��ĕԂ�(�����`��)
std::vector<std::vector<int>> CSVRead(const char *DatabaseName){
	//�J���}��؂�œǂݎ��
	std::vector<std::vector<std::string>> DatabaseStr=CommaCutRead(DatabaseName);
	//int�^�ɒ���
	std::vector<std::vector<int>> Database;
	for(unsigned int i=0;i<DatabaseStr.size();i++){
		std::vector<int> Data;//�����ɂP�s��������
		for(unsigned int n=0;n<DatabaseStr[i].size();n++){//i�s�ڂ̃f�[�^�����
			Data.push_back(atoi(DatabaseStr[i][n].c_str()));
		}
		Database.push_back(Data);
	}
	//�ǂݎ�������̂�Ԃ�
	return Database;
}
