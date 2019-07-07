#ifndef DEF_STAGEINFOINSTAGESELECT_H
#define DEF_STAGEINFOINSTAGESELECT_H

#include<string>
#include"ScoreRankingData.h"
#include"ToolsLib.h"

//ステージセレクト場面におけるステージ情報
struct StageInfoInStageSelect{
	//変数
	int m_mapPic;//マップグラフィック(縮小表示)(push_back()の際にデストラクタが呼ばれグラフィックが消されるので、削除はデストラクタでは行わない。どうしてもデストラクタでしたくなったら、コピーコンストラクタを作って再度CopyGraph()をしよう。)
	std::string m_dirName;//ディレクトリ名
	std::string m_titleName;//ステージ名
	std::string m_explain;//ステージ説明文
	ScoreRankingData::StageScoreData m_rankingVec;//ランキングデータ
	Vector2D m_pos;//ボタンの位置

	//静的変数
	static const int boxWidth;
	static const int boxHeight;

	//関数
	StageInfoInStageSelect(const int mapPic,const std::string &dirName,const std::string &explain,const ScoreRankingData &rankingData);
	~StageInfoInStageSelect();
	void DrawInfo(const int centerX,const int centerY,const int nameFont,const int explainFont)const;//ステージ名とマップ絵、説明文を描画する
};


#endif // !DEF_STAGEINFOINSTAGESELECT_H