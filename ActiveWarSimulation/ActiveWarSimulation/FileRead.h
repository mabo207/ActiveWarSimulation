#ifndef DEF_FILEREAD_H
#define DEF_FILEREAD_H

//�C���N���[�h��
#include<vector>
#include<string>

//�֐�
//�t�@�C����ǂݍ���string�^�ɂ��ĕԂ�
std::string FileStrRead(const char *DatabaseName);

//�t�@�C����ǂݍ��݁A���s�E�J���}�ŋ�؂�ꂽ��������ʂ��ĕԂ�
std::vector<std::vector<std::string>> CommaCutRead(const char *DatabaseName);

//CSV�t�H���_��ǂݎ����vector<vector<int>>�^�ɂ��ĕԂ�
std::vector<std::vector<int>> CSVRead(const char *DatabaseName);

#endif