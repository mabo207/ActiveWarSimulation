#ifndef DEF_FILEREAD_H
#define DEF_FILEREAD_H

//�C���N���[�h��
#include<vector>
#include<string>
#include<map>

using namespace std;

//�֐�
//�t�@�C����ǂݍ���string�^�ɂ��ĕԂ�
string FileStrRead(const char *DatabaseName);

//CSV�t�H���_��ǂݎ����vector<vector<int>>�^�ɂ��ĕԂ�(�����`��)
vector<vector<int>> CSVRead(const char *DatabaseName);

#endif