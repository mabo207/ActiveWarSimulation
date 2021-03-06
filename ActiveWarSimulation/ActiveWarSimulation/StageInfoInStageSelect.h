#ifndef DEF_STAGEINFOINSTAGESELECT_H
#define DEF_STAGEINFOINSTAGESELECT_H

#include<string>
#include"ScoreRankingData.h"
#include"ToolsLib.h"

//ステージセレクト場面におけるステージ情報
struct StageInfoInStageSelect{
	//変数
	int m_mapPicRef;//マップグラフィック(縮小表示)(参照)
	std::string m_dirName;//ディレクトリ名
	std::string m_titleName;//ステージ名
	std::string m_explain;//ステージ説明文
	ScoreRankingData::StageScoreData m_rankingVec;//ランキングデータ
	Vector2D m_pos;//ボタンの位置

	//静的変数
	static const int stageBoxWidth;
	static const int stageBoxHeight;
	static const int levelBoxWidth;
	static const int levelBoxHeight;

	//関数
	StageInfoInStageSelect(const int mapPic,const std::string &dirName,const std::string &explain,const ScoreRankingData &rankingData);
	~StageInfoInStageSelect();
	void DrawStageInfo(const int centerX,const int centerY,const int nameFont,const int explainFont)const;//ステージ名とマップ絵、説明文を描画する
	void DrawLevelInfo(const StageLevel level,const int x,const int y,const int levelNameFont,const int rankingFont)const;//指定されたレベルについて、レベル文字列とランキングを表示
};


#endif // !DEF_STAGEINFOINSTAGESELECT_H