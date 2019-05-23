#ifndef DEF_FILEREAD_H
#define DEF_FILEREAD_H

//インクルード等
#include<vector>
#include<string>
#include<map>

using namespace std;

//関数
//ファイルを読み込みstring型にして返す
string FileStrRead(const char *DatabaseName);

//CSVフォルダを読み取ってvector<vector<int>>型にして返す(特化形式)
vector<vector<int>> CSVRead(const char *DatabaseName);

#endif