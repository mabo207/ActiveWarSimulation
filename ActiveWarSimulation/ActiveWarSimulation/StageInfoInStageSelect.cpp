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
const int StageInfoInStageSelect::boxWidth=picWidth+offsetLeft*2;
const int StageInfoInStageSelect::boxHeight=explainDY+120+offsetTop;

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

void StageInfoInStageSelect::DrawInfo(const int centerX,const int centerY,const int nameFont,const int explainFont)const{
	const int x=centerX-boxWidth/2,y=centerY-boxHeight/2;//����̍��W���v�Z
	//�`��
	DrawBox(x,y,x+boxWidth,y+boxHeight,GetColor(32,64,32),TRUE);
	DrawGraph(x+offsetLeft,y+offsetTop,m_mapPic,TRUE);
	DrawStringCenterBaseToHandle(x+boxWidth/2,y+stageNameDY,m_titleName.c_str(),GetColor(255,255,255),nameFont,false);
	DrawStringNewLineToHandle(x+offsetLeft,y+explainDY,boxWidth,300,m_explain.c_str(),GetColor(255,255,255),explainFont,2);
}
