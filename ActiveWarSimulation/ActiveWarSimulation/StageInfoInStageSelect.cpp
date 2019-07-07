#include"StageInfoInStageSelect.h"
#include"StageInfoReader.h"
#include"DxLib.h"
#include"CommonConstParameter.h"

namespace {
	//m_mapPicは加工されて画面全体の縦横共に1/4の大きさになっていることを用いる
	const int offsetLeft=10,offsetTop=10,offsetSpace=20;
	const int picWidth=CommonConstParameter::gameResolutionX/4,picHeight=CommonConstParameter::gameResolutionY/4;
	const int stageNameDY=offsetTop+picHeight+offsetSpace;//ステージ描画UIの上端からどの位置にステージ名を配置するか
	const int explainDY=stageNameDY+32+offsetSpace;//ステージ描画UIの上端からどの位置に説明文を配置するか
}

//----------------StageInfoInStageSelect--------------------
const int StageInfoInStageSelect::boxWidth=picWidth+offsetLeft*2;
const int StageInfoInStageSelect::boxHeight=explainDY+120+offsetTop;

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

void StageInfoInStageSelect::DrawInfo(const int centerX,const int centerY,const int nameFont,const int explainFont)const{
	const int x=centerX-boxWidth/2,y=centerY-boxHeight/2;//左上の座標を計算
	//描画
	DrawBox(x,y,x+boxWidth,y+boxHeight,GetColor(32,64,32),TRUE);
	DrawGraph(x+offsetLeft,y+offsetTop,m_mapPic,TRUE);
	DrawStringCenterBaseToHandle(x+boxWidth/2,y+stageNameDY,m_titleName.c_str(),GetColor(255,255,255),nameFont,false);
	DrawStringNewLineToHandle(x+offsetLeft,y+explainDY,boxWidth,300,m_explain.c_str(),GetColor(255,255,255),explainFont,2);
}
