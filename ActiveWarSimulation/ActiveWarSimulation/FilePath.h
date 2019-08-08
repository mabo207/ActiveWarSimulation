#ifndef DEF_FILEPATH_H
#define DEF_FILEPATH_H

//���ʂ��ėp����t�@�C���p�X�����ʊǗ����邽�߂̃w�b�_�[�t�@�C��
#include<string>

struct FilePath{
	static const std::string stageDir;
	static const std::string fontDir;
	static const std::string graphicDir;
	static const std::string bgmDir;
	static const std::string effectSoundDir;
	static const std::string bgmInfoDir;
	static const std::string savedataDir;
};

#endif // !DEF_FILEPATH_H