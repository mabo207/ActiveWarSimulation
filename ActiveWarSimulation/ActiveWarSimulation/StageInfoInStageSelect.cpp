#include"StageInfoInStageSelect.h"
#include"StageInfoReader.h"
#include"DxLib.h"
#include"CommonConstParameter.h"

namespace {
	//m_mapPic�͉��H����ĉ�ʑS�̂̏c������1/4�̑傫���ɂȂ��Ă��邱�Ƃ�p����
	const int offsetLeft=10,offsetTop=10,offsetSpace=20;
	const int picWidth=CommonConstParameter::gameResolutionX/4,picHeight=CommonConstParameter::gameResolutionY/4;
	const int stageNameDY=offsetTop+picHeight+offsetSpace;//�X�e�[�W�`��UI�̏�[����ǂ̈ʒu�ɃX�e�[�W����z�u���邩
	const int explainDY=stageNameDY+32+offsetSpace;//�X�e�[�W�`��UI�̏�[����ǂ̈ʒu�ɐ�������z�u���邩
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
	//�X�e�[�W���̓ǂݎ��
	const StageInfoReader reader(dirName);
	m_titleName=reader.GetTitleName();
	m_pos=reader.GetPos();
}

StageInfoInStageSelect::~StageInfoInStageSelect(){
	//DeleteGraphEX(m_mapPic);
}

void StageInfoInStageSelect::DrawStageInfo(const int centerX,const int centerY,const int nameFont,const int explainFont)const{
	const int x=centerX-stageBoxWidth/2,y=centerY-stageBoxHeight/2;//����̍��W���v�Z
	//�`��
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
	//�w�i�̕`��
	//m_levelButton[i].DrawButtonRect(GetColor(64,32,32),TRUE);
	DrawBox(x,y,x+levelBoxWidth,y+levelBoxHeight,GetColor(64,32,32),TRUE);
	//���x�����̕`��
	DrawStringToHandle(x+5,y+5,level.GetString().c_str(),GetColor(255,255,255),levelNameFont);
	//�����L���O�f�[�^�ꗗ
	const std::map<StageLevel,ScoreRankingData::LevelData>::const_iterator itLevel=m_rankingVec.levelMap.find(level);
	size_t counter=0;
	if(itLevel!=m_rankingVec.levelMap.end()){
		//�����L���O�f�[�^�����݂���ꍇ�̓f�[�^��`��
		std::set<ScoreRankingData::PlayerData>::const_iterator itPlayer;
		for(const ScoreRankingData::PlayerData &data:itLevel->second.playerDataSet){
			//���O
			DrawStringToHandle(x+nameX,y+rankingY,data.name.c_str(),GetColor(255,255,255),rankingFont);
			//�_��
			DrawStringRightJustifiedToHandle(x+scoreX,y+rankingY,to_string_0d(data.score,7),GetColor(255,255,255),rankingFont);
			//�ʒu���炵
			rankingY+=rankingFontSize;
			//���𑝂₵�āArankingSize�ȏ�̕`��ɂȂ�����`���ł��؂�
			counter++;
			if(counter>=rankingSize){
				break;
			}
		}
	}
	for(;counter<rankingSize;counter++){
		//����Ȃ�����-----��`��
		//���O
		DrawStringToHandle(x+nameX,y+rankingY,"----------",GetColor(255,255,255),rankingFont);
		//�_��
		DrawStringRightJustifiedToHandle(x+scoreX,y+rankingY,"-------",GetColor(255,255,255),rankingFont);
		//�ʒu���炵
		rankingY+=rankingFontSize;
	}
}
