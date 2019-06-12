#ifndef DEF_FILEPATH_H
#define DEF_FILEPATH_H

//共通して用いるファイルパスを共通管理するためのヘッダーファイル
#include<string>

struct FilePath{
	static const std::string stageDir;
	static const std::string fontDir;
	static const std::string graphicDir;
	static const std::string bgmDir;
	static const std::string effectSoundDir;
};

#endif // !DEF_FILEPATH_H