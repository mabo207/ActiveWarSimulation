#include"StageInfoInStageSelect.h"
#include"StageInfoReader.h"
#include"DxLib.h"
#include"CommonConstParameter.h"

//----------------StageInfoInStageSelect--------------------
StageInfoInStageSelect::StageInfoInStageSelect(const int mapPic,const std::string &dirName,const std::string &explain,const ScoreRankingData &rankingData)
	:m_mapPic(mapPic)
	,m_dirName(dirName)
	,m_explain(explain)
	,m_rankingVec(rankingData.GetStageScoreData(dirName))
{
	//ステージ情報の読み取り
	const StageInfoReader reader(dirName);
	m_titleName=reader.GetTitleName();
	m_pos=reader.GetPos();
}

StageInfoInStageSelect::~StageInfoInStageSelect(){
	//DeleteGraphEX(m_mapPic);
}

void StageInfoInStageSelect::DrawInfo(const int x,const int y,const int nameFont,const int explainFont)const{
	//m_mapPicは加工されて画面全体の縦横共に1/4の大きさになっていることを用いる
	const int offsetLeft=10,offsetTop=10;
	const int stageNameY=y+offsetTop+CommonConstParameter::gameResolutionY/4+20;
	const int explainY=stageNameY+GetFontSizeToHandle(nameFont)+20;
	//描画
	DrawGraph(x+offsetLeft,y+offsetTop,m_mapPic,TRUE);
	DrawStringCenterBaseToHandle(x+offsetLeft+CommonConstParameter::gameResolutionX/8,stageNameY,m_titleName.c_str(),GetColor(255,255,255),nameFont,false);
	DrawStringNewLineToHandle(x+offsetLeft,explainY,CommonConstParameter::gameResolutionX/4,300,m_explain.c_str(),GetColor(255,255,255),explainFont,2);
}
