#ifndef DEF_STAGEINFOREADER_H
#define DEF_STAGEINFOREADER_H

//どのシーンでも呼んでも良いよ、という関数・クラス
//このソリューションに使用が限られるのでToolsフォルダに入れられないが、色々な箇所で使いたい処理をここに書く。

#include<string>

//"Stage/hoge/stageInfo.txt"から、特定の値を取り出すためのクラス。
class StageInfoReader{
public:
	StageInfoReader(const std::string &dirName);
	~StageInfoReader();
	std::string GetTitleName()const{
		return titleName;
	}
	int GetX()const{
		return x;
	}
	int GetY()const{
		return y;
	}

private:
	std::string titleName;
	int x,y;
};

#endif // !DEF_STAGEINFOREADER_H