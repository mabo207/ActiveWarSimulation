#ifndef DEF_STAGEINFOREADER_H
#define DEF_STAGEINFOREADER_H

//どのシーンでも呼んでも良いよ、という関数・クラス
//このソリューションに使用が限られるのでToolsフォルダに入れられないが、色々な箇所で使いたい処理をここに書く。

#include<string>
#include"ToolsLib.h"

//"Stage/hoge/stageInfo.txt"から、特定の値を取り出すためのクラス。
class StageInfoReader{
public:
	StageInfoReader(const std::string &dirName);
	~StageInfoReader();
	std::string GetTitleName()const{
		return titleName;
	}
	Vector2D GetPos()const{
		return pos;
	}
	std::string GetBgmInfoFileName()const{
		return bgmFolderName;
	}

private:
	std::string titleName;
	Vector2D pos;
	std::string bgmFolderName;
};

#endif // !DEF_STAGEINFOREADER_H