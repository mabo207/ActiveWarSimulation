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
const int StageInfoInStageSelect::stageBoxWidth=picWidth+offsetLeft*2;
const int StageInfoInStageSelect::stageBoxHeight=explainDY+120+offsetTop;
const int StageInfoInStageSelect::levelBoxWidth=StageInfoInStageSelect::stageBoxWidth;
const int StageInfoInStageSelect::levelBoxHeight=CommonConstParameter::gameResolutionY/4-70;

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

void StageInfoInStageSelect::DrawStageInfo(const int centerX,const int centerY,const int nameFont,const int explainFont)const{
	const int x=centerX-stageBoxWidth/2,y=centerY-stageBoxHeight/2;//左上の座標を計算
	//描画
	DrawBox(x,y,x+stageBoxWidth,y+stageBoxHeight,GetColor(32,64,32),TRUE);
	DrawGraph(x+offsetLeft,y+offsetTop,m_mapPic,TRUE);
	DrawStringCenterBaseToHandle(x+stageBoxWidth/2,y+stageNameDY,m_titleName.c_str(),GetColor(255,255,255),nameFont,false);
	DrawStringNewLineToHandle(x+offsetLeft,y+explainDY,stageBoxWidth,300,m_explain.c_str(),GetColor(255,255,255),explainFont,2);
}

void StageInfoInStageSelect::DrawLevelInfo(const StageLevel level,const int x,const int y,const int levelNameFont,const int rankingFont)const{
	const size_t rankingSize=5;
	const int nameX=10,scoreX=350;
	int rankingY=40;
	const int rankingFontSize=GetFontSizeToHandle(rankingFont);
	//背景の描画
	//m_levelButton[i].DrawButtonRect(GetColor(64,32,32),TRUE);
	DrawBox(x,y,x+levelBoxWidth,y+levelBoxHeight,GetColor(64,32,32),TRUE);
	//レベル名の描画
	DrawStringToHandle(x+5,y+5,level.GetString().c_str(),GetColor(255,255,255),levelNameFont);
	//ランキングデータ一覧
	const std::map<StageLevel,ScoreRankingData::LevelData>::const_iterator itLevel=m_rankingVec.levelMap.find(level);
	size_t counter=0;
	if(itLevel!=m_rankingVec.levelMap.end()){
		//ランキングデータが存在する場合はデータを描画
		std::set<ScoreRankingData::PlayerData>::const_iterator itPlayer;
		for(const ScoreRankingData::PlayerData &data:itLevel->second.playerDataSet){
			//名前
			DrawStringToHandle(x+nameX,y+rankingY,data.name.c_str(),GetColor(255,255,255),rankingFont);
			//点数
			DrawStringRightJustifiedToHandle(x+scoreX,y+rankingY,to_string_0d(data.score,7),GetColor(255,255,255),rankingFont);
			//位置ずらし
			rankingY+=rankingFontSize;
			//個数を増やして、rankingSize個以上の描画になったら描画を打ち切る
			counter++;
			if(counter>=rankingSize){
				break;
			}
		}
	}
	for(;counter<rankingSize;counter++){
		//足りない分は-----を描画
		//名前
		DrawStringToHandle(x+nameX,y+rankingY,"----------",GetColor(255,255,255),rankingFont);
		//点数
		DrawStringRightJustifiedToHandle(x+scoreX,y+rankingY,"-------",GetColor(255,255,255),rankingFont);
		//位置ずらし
		rankingY+=rankingFontSize;
	}
}
