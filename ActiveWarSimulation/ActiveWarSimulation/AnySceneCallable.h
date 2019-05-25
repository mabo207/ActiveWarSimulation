#ifndef DEF_ANYSCENECALLABLE_H
#define DEF_ANYSCENECALLABLE_H

//どのシーンでも呼んでも良いよ、という関数・クラス
//このソリューションに使用が限られるのでToolsフォルダに入れられないが、色々な箇所で使いたい処理をここに書く。

#include"ToolsLib.h"

//"Stage/hoge/stageInfo.txt"から、特定の値を取り出すためのクラス。
class StageInfoReader{
public:
	StageInfoReader(const std::string &dirName);
	~StageInfoReader();
	int GetLevel()const{
		return stageLevel;
	}
	std::string GetTitleName()const{
		return titleName;
	}

private:
	int stageLevel;
	std::string titleName;
};

#endif // !DEF_ANYSCENECALLABLE_H