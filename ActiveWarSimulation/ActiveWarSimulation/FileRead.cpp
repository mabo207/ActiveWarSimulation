#include<iostream>
#include<fstream>
#include<cassert>
#include"FileRead.h"




//ファイルを読み込みvector<char>型にして返す
vector<char> FileRead(const char *DatabaseName){
	//ファイルを開く
	ifstream ifs(DatabaseName);
	if(!ifs){
		assert(false);
		return vector<char>();
	}
	//文字を読み込む
	vector<char> OriginalStr;
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

//ファイルを読み込み、改行・カンマで区切られた部分を区別して返す(特化形式)
vector<vector<string>> CommaCutRead(const char *DatabaseName){
	//ファイルの読み込み
	vector<char> OriginalData=FileRead(DatabaseName);
	//改行毎にデータを分けて、１行内についてカンマ区切りで文字列を読み取る
	vector<vector<string>> CutData;//こいつを返す
	vector<string> LineData;//１行分のデータ
	string Str="";//LineDataに入れる文字列
	for(unsigned int i=0;i<OriginalData.size();i++){
		switch(OriginalData[i]){
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
			Str=Str+OriginalData[i];//文字列に文字を追加
			break;
		}
	}
	return CutData;
}

//ファイルを読み込みstring型にして返す
string FileStrRead(const char *DatabaseName){
	//ファイルの読み込み
	vector<char> OriginalData=FileRead(DatabaseName);
	//string型に直す
	string Str;
	for(unsigned int i=0;i<OriginalData.size();i++){
		Str=Str+OriginalData[i];
	}
	return Str;
}

//文字列をsplitで分割し全て返す
vector<string> StrTok(const string OriginalStr,const char split){
	//OriginalStrをsplitで分割してCutStrに格納する
	vector<string> CutStr;
	for(unsigned int i=0;i<OriginalStr.size();i++){
		static string CutData;//ここに文字を追加していき、splitが来たらCutStrに格納する
		//OriginalStrのi番目の文字を見る
		if(OriginalStr[i]==split){
			//CutData=CutData+'\0';
			CutStr.push_back(CutData);
			CutData.clear();
		}else{
			CutData=CutData+OriginalStr[i];
			if(i==OriginalStr.size()-1){//文字列が終わる時
				//CutData=CutData+'\0';
				CutStr.push_back(CutData);
				CutData.clear();
			}
		}
	}
	return CutStr;
}

//ファイルをsplit1で分割して読み込んだ後、残りをsplit2で分割。split2分割に対して最初の文字列をkeyとしたmapを返す
map<string,vector<string>> ReadDoubleSplitToMap(const char *DatabaseName,const char split1,const char split2){
	//split1で分割読み込み
	vector<string> vs=StrTok(FileStrRead(DatabaseName),split1);
	//split2で分割読み込み
	map<string,vector<string>> rmap;
	for(unsigned int i=0;i<vs.size();i++){
		vector<string> vvs=StrTok(vs[i],split2);
		//mapに格納するのはsplit2で2個以上に分割できたときのみ
		if(vvs.size()>1){
			//先頭要素を別に救出してから削除
			string s=vvs[0];
			vvs.erase(vvs.begin());
			//rmapに格納
			rmap.insert(pair<string,vector<string>>(s,vvs));
		}
	}
	return rmap;
}

//文字列をカンマで分割し、先頭要素をkeyとしてmapにして返す(特化形式)
map<string,vector<string>> CommaCutReadMap(const char *DatabaseName){
	//カンマ分割読み込み
	vector<vector<string>> vvs=CommaCutRead(DatabaseName);
	//mapに格納
	map<string,vector<string>> map;
	for(unsigned int i=0;i<vvs.size();i++){
		if(vvs[i].size()>1){
			string key=vvs[i][0];
			vvs[i].erase(vvs[i].begin());
			map.insert(pair<string,vector<string>>(key,vvs[i]));
		}
	}
	return map;
}

//CSVフォルダを読み取ってvector<vector<int>>型にして返す(特化形式)
vector<vector<int>> CSVRead(const char *DatabaseName){
	//カンマ区切りで読み取る
	vector<vector<string>> DatabaseStr=CommaCutRead(DatabaseName);
	//int型に直す
	vector<vector<int>> Database;
	for(unsigned int i=0;i<DatabaseStr.size();i++){
		vector<int> Data;//ここに１行分を入れる
		for(unsigned int n=0;n<DatabaseStr[i].size();n++){//i行目のデータを作る
			Data.push_back(atoi(DatabaseStr[i][n].c_str()));
		}
		Database.push_back(Data);
	}
	//読み取ったものを返す
	return Database;
}

//テキストデータ、CSVデータのどちらでもいいが、武器などの説明文を読み取る関数。(特化形式)
//処理が重いので処理方法を変えている
string ExplainDataRead(const char *DatabaseName,const char *index,const char smallsplit,const char largesplit){
	//ここではsmallsplit==',',largesplit=='$'として説明する
	//$に囲われた部分の中を,で区切り、区切った複数の文字列の内、
	//1番目がindexに一致すれば2番目の文字列を返す
	
	//ファイルを開く
	ifstream ifs(DatabaseName);
	if(!ifs){
		assert(false);
		return "";
	}
	//文字を読み込む
	string ReturnStr;//こいつを返す
	string indexstr;//ここに区切りの1番目を入れる
	int nowprocess=0;//作業段階 0:$を探す 1:区切りの1番目を作る 2:ReturnStrの作成
	bool roopflag=true;//作業が終わっていないかどうか
	while(roopflag){
		char word=ifs.get();
		if(word==EOF){//読み取ったものがEOFになるまでループする
			break;
		}
		switch(nowprocess){
		case(0)://0:$を探す
			if(word==largesplit){
				nowprocess=1;//無条件で次の段階に進む
			}
			break;
		case(1)://1:区切りの1番目を作る
			if(word==smallsplit){//,1番目の区切りが完成したならば
				if(indexstr==index){//1番目の区切りがindexに一致すれば
					nowprocess=2;//次の段階に進む
				}else{
					nowprocess=0;//$探しからやり直す
				}
				indexstr.clear();//indexstrを初期化する
			}else if(word==largesplit){//,の区切りを見つける前に$の区切りを見つけたなら
				indexstr.clear();//何もない状態からやり直す
			}else{
				indexstr=indexstr+word;
			}
			break;
		case(2)://2:ReturnStrの作成
			if(word==smallsplit || word==largesplit){//区切りを見つけたら
				roopflag=false;//作成終了
			}else{
				ReturnStr=ReturnStr+word;
			}
			break;

		}
	}

	//ファイルを閉じ関数を終了する
	ifs.close();

	return ReturnStr;
}

