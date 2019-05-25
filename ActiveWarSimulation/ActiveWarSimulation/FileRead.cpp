#include<iostream>
#include<fstream>
#include<cassert>
#include"FileRead.h"
#include<climits>


//ファイルを読み込みstd::string型にして返す
std::string FileStrRead(const char *DatabaseName){
	//ファイルを開く
	std::ifstream ifs(DatabaseName);
	if(!ifs){
		assert(false);
		return "";
	}
	//ファイルサイズを取得
	ifs.seekg(0,std::ifstream::end);
	const auto endPos=ifs.tellg();//末尾の位置を取得
	ifs.clear();
	ifs.seekg(0,std::ifstream::beg);
	const auto beginPos=ifs.tellg();//先頭の位置を取得
	const auto fileSize=endPos-beginPos;//ファイルサイズが分かる(byte)
	//文字を読み込む
	std::string OriginalStr;
	if(fileSize>=0 && fileSize<UINT_MAX){
		OriginalStr.reserve(static_cast<size_t>(fileSize));//ファイルサイズ分だけメモリ確保
	}
	while(true){
		char word=ifs.get();
		if(word==EOF){//読み取ったものがEOFになるまでループする
			break;
		}
		OriginalStr.push_back(word);
	}

	//ファイルを閉じる
	ifs.close();

	return OriginalStr;
}

//ファイルを読み込み、改行・カンマで区切られた部分を区別して返す
std::vector<std::vector<std::string>> CommaCutRead(const char *DatabaseName){
	//ファイルの読み込み
	const std::string OriginalData=FileStrRead(DatabaseName);
	//改行毎にデータを分けて、１行内についてカンマ区切りで文字列を読み取る
	std::vector<std::vector<std::string>> CutData;//こいつを返す
	std::vector<std::string> LineData;//１行分のデータ
	std::string Str="";//LineDataに入れる文字列
	for(const char c:OriginalData){
		switch(c){
		case('\n')://改行文字の時
			if(Str!=""){
				LineData.push_back(Str);//Strに何か書いてあればLineDataに加える
			}
			CutData.push_back(LineData);//CutDataに挿入
			//初期化
			LineData.clear();
			Str.clear();
			break;
		case(',')://カンマの時
			LineData.push_back(Str);//LineDataに挿入
			//初期化
			Str.clear();
			break;
		default://それ以外の時
			Str.push_back(c);//文字列に文字を追加
			break;
		}
	}
	return CutData;
}

//CSVフォルダを読み取ってstd::vector<std::vector<int>>型にして返す
std::vector<std::vector<int>> CSVRead(const char *DatabaseName){
	//カンマ区切りで読み取る
	std::vector<std::vector<std::string>> DatabaseStr=CommaCutRead(DatabaseName);
	//int型に直す
	std::vector<std::vector<int>> Database;
	for(unsigned int i=0;i<DatabaseStr.size();i++){
		std::vector<int> Data;//ここに１行分を入れる
		for(unsigned int n=0;n<DatabaseStr[i].size();n++){//i行目のデータを作る
			Data.push_back(atoi(DatabaseStr[i][n].c_str()));
		}
		Database.push_back(Data);
	}
	//読み取ったものを返す
	return Database;
}
