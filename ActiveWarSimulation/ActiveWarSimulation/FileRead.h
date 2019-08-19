#ifndef DEF_FILEREAD_H
#define DEF_FILEREAD_H

//インクルード等
#include<vector>
#include<string>
#include<exception>

//ファイル読み込み失敗時の例外
class FileOpenFailedException:public std::exception{
public:
	FileOpenFailedException()=default;
	~FileOpenFailedException()=default;
	const char *what()const noexcept;
};

//関数
//ファイルを読み込みstring型にして返す
std::string FileStrRead(const char *DatabaseName);

//ファイルを読み込み、改行・カンマで区切られた部分を区別して返す
std::vector<std::vector<std::string>> CommaCutRead(const char *DatabaseName);

//CSVフォルダを読み取ってvector<vector<int>>型にして返す
std::vector<std::vector<int>> CSVRead(const char *DatabaseName);

#endif