#ifndef DEF_FILEREAD_H
#define DEF_FILEREAD_H

//インクルード等
#include<vector>
#include<string>
#include<map>

using namespace std;

//関数
//ファイルを読み込みvector<char>型にして返す
vector<char> FileRead(const char *DatabaseName);

//ファイルを読み込みstring型にして返す
string FileStrRead(const char *DatabaseName);

//ファイルを読み込み、改行・カンマで区切られた部分を区別して返す(特化形式)
vector<vector<string>> CommaCutRead(const char *DatabaseName);

//文字列をsplitで分割し全て返す
vector<string> StrTok(const string OriginalStr,const char split);

//ファイルをsplit1で分割して読み込んだ後、残りをsplit2で分割。split2分割に対して最初の文字列をkeyとしたmapを返す
map<string,vector<string>> ReadDoubleSplitToMap(const char *DatabaseName,const char split1,const char split2);

//文字列をカンマで分割し、先頭要素をkeyとしてmapにして返す(特化形式)
map<string,vector<string>> CommaCutReadMap(const char *DatabaseName);

//CSVフォルダを読み取ってvector<vector<int>>型にして返す(特化形式)
vector<vector<int>> CSVRead(const char *DatabaseName);

//テキストデータ、CSVデータのどちらでもいいが、武器などの説明文を読み取る関数(特化形式)
//処理が重いので処理方法を変えている
string ExplainDataRead(const char *DatabaseName,const char *index,const char smallsplit,const char largesplit='\0');


#endif