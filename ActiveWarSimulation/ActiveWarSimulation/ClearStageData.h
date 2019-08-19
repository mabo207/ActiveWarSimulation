#ifndef DEF_CLEARSTAGEDATA_H
#define DEF_CLEARSTAGEDATA_H

#include<string>
#include<set>

//クリアステージの一覧データ
class ClearStageData{
public:
	ClearStageData();
	~ClearStageData()=default;
	bool JudgeClear(const std::string &stageDirName)const;//stageDirNameがクリアしているかどうか
	void BecomeClear(const std::string &stageDirName);//stageDirNameがクリアしたものとする
	bool Save()const;//現在のm_clearDirをセーブする

protected:
	std::set<std::string> m_clearDir;//クリアしたステージのディレクトリ一覧
};

#endif // !DEF_CLEARSTAGEDATA_H
