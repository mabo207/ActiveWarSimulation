#ifndef DEF_FILEREAD_H
#define DEF_FILEREAD_H

//インクルード等
#include<vector>
#include<string>
#include<memory>

//関数
//ファイルを読み込みstring型にして返す
std::string FileStrRead(const char *DatabaseName);

//CSVフォルダを読み取ってvector<vector<int>>型にして返す(特化形式)
std::vector<std::vector<int>> CSVRead(const char *DatabaseName);

#endif