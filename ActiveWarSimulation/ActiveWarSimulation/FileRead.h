#ifndef DEF_FILEREAD_H
#define DEF_FILEREAD_H

//�C���N���[�h��
#include<vector>
#include<string>
#include<memory>

//�֐�
//�t�@�C����ǂݍ���string�^�ɂ��ĕԂ�
std::string FileStrRead(const char *DatabaseName);

//CSV�t�H���_��ǂݎ����vector<vector<int>>�^�ɂ��ĕԂ�(�����`��)
std::vector<std::vector<int>> CSVRead(const char *DatabaseName);

#endif