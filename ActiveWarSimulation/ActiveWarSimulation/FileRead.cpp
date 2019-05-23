#include<iostream>
#include<fstream>
#include<cassert>
#include"FileRead.h"


//�t�@�C����ǂݍ���vector<char>�^�ɂ��ĕԂ�
vector<char> FileRead(const char *DatabaseName){
	//�t�@�C�����J��
	ifstream ifs(DatabaseName);
	if(!ifs){
		assert(false);
		return vector<char>();
	}
	//������ǂݍ���
	vector<char> OriginalStr;
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
vector<vector<string>> CommaCutRead(const char *DatabaseName){
	//�t�@�C���̓ǂݍ���
	vector<char> OriginalData=FileRead(DatabaseName);
	//���s���Ƀf�[�^�𕪂��āA�P�s���ɂ��ăJ���}��؂�ŕ������ǂݎ��
	vector<vector<string>> CutData;//������Ԃ�
	vector<string> LineData;//�P�s���̃f�[�^
	string Str="";//LineData�ɓ���镶����
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

//�t�@�C����ǂݍ���string�^�ɂ��ĕԂ�
string FileStrRead(const char *DatabaseName){
	//�t�@�C���̓ǂݍ���
	vector<char> OriginalData=FileRead(DatabaseName);
	//string�^�ɒ���
	string Str;
	for(unsigned int i=0;i<OriginalData.size();i++){
		Str=Str+OriginalData[i];
	}
	return Str;
}

//CSV�t�H���_��ǂݎ����vector<vector<int>>�^�ɂ��ĕԂ�(�����`��)
vector<vector<int>> CSVRead(const char *DatabaseName){
	//�J���}��؂�œǂݎ��
	vector<vector<string>> DatabaseStr=CommaCutRead(DatabaseName);
	//int�^�ɒ���
	vector<vector<int>> Database;
	for(unsigned int i=0;i<DatabaseStr.size();i++){
		vector<int> Data;//�����ɂP�s��������
		for(unsigned int n=0;n<DatabaseStr[i].size();n++){//i�s�ڂ̃f�[�^�����
			Data.push_back(atoi(DatabaseStr[i][n].c_str()));
		}
		Database.push_back(Data);
	}
	//�ǂݎ�������̂�Ԃ�
	return Database;
}
