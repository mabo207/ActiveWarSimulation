#include"DxLib.h"
#include"SelfDecideSubmission.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"ToolsLib.h"
#include"GraphicControl.h"

#include"ArcherAttackDistance.h"

namespace{
	const int wholeCommentWidth=500;
	const int wholeCommentHeight=200;
}

//--------------SelfDecideSubmission---------------
const int SelfDecideSubmission::s_submissionWidth=500;
const int SelfDecideSubmission::s_submissionHeight=60;

SelfDecideSubmission::SelfDecideSubmission(const std::shared_ptr<SubmissionRuleBase> &rule)
	:m_rule(rule)
	,m_rubricList()
	,m_wholeComment()
	,m_sentenceFont(CreateFontToHandleEX("���C���I",24,2,DX_FONTTYPE_NORMAL))
	,m_rubricFont(CreateFontToHandleEX("���C���I",20,2,DX_FONTTYPE_EDGE,-1,2))
{}

SelfDecideSubmission::~SelfDecideSubmission(){
	DeleteFontToHandleEX(m_sentenceFont);
	DeleteFontToHandleEX(m_rubricFont);
}

bool SelfDecideSubmission::JudgeEvaluatedOrder(const BattleSceneData * const battleData)const{
	return m_rule->JudgeEvaluateOrder(battleData);
}

void SelfDecideSubmission::RubricEvaluate(const BattleSceneData * const battleData){
	//�]���̒~��
	m_rubricList.push_back(m_rule->RubricEvaluate(battleData));
}

void SelfDecideSubmission::WholeLookBack(){
	//Rule�N���X�Ɉڏ�
	if(m_rubricList.empty()){
		//1����]���ΏۂƂȂ�s�������Ă��Ȃ���
		m_wholeComment=m_rule->GetWholeLookBackActionEmpty();
		return;
	}
	//�ŕp�l�����߂�
	std::map<int,size_t> frequencyMap;
	for(const int &rubric:m_rubricList){
		std::map<int,size_t>::iterator it=frequencyMap.find(rubric);
		if(it==frequencyMap.end()){
			//�܂�rubric��1��������Ă��Ȃ��ꍇ�́AfrequencyMap�ɒǉ�
			frequencyMap.insert(std::make_pair(rubric,1));
		} else{
			//���Ɍ������Ă���ꍇ�́A�񐔂�1���₷
			it->second++;
		}
	}
	std::pair<int,size_t> mostFrequent=*frequencyMap.begin();
	for(const std::pair<int,size_t> &pair:frequencyMap){
		if(mostFrequent.first==-1){
			//mostFrequent���u�U�����Ă��Ȃ��v�ł���ꍇ�́A�K���㏑������
			mostFrequent=pair;
		} else if(pair.second>mostFrequent.second && pair.first!=-1){
			//�u�U�����Ă��Ȃ��v�ȊO�̕p�x���傫�����̂ɍX�V
			mostFrequent=pair;
		}
	}
	//�ŕp�l�̃��[�u���b�N�ɉ����ăR�����g���i�[
	m_wholeComment=m_rule->GetWholeLookBack(mostFrequent.first);
}

void SelfDecideSubmission::DrawSubmission(int x,int y)const{
	//������w���Rule�Ɉڏ�
	const std::string submissionStr=m_rule->GetSubmissionExplanation();
	DrawBox(x,y,x+s_submissionWidth,y+s_submissionHeight,GetColor(64,192,64),TRUE);
	DrawBox(x,y,x+s_submissionWidth,y+s_submissionHeight,GetColor(192,255,192),FALSE);
	DrawStringNewLineToHandle(x+5,y+5,s_submissionWidth-10,s_submissionHeight-10,submissionStr.c_str(),GetColor(255,255,255),m_sentenceFont,2);
}

void SelfDecideSubmission::DrawRubric(int centerX,int centerY)const{
	if(!m_rubricList.empty()){
		//���[�u���b�N�]���̕������`(Rule�Ɉڏ�)
		std::pair<std::string,unsigned int> pair=m_rule->GetRubricStringInfo(m_rubricList.back());
		std::string rubricStr=pair.first;
		unsigned int edgeColor=pair.second;
		//�`��
		DrawStringCenterBaseToHandle(centerX,centerY,rubricStr.c_str(),GetColor(255,255,255),m_rubricFont,true,edgeColor);
	}
}

void SelfDecideSubmission::DrawWholeLookBack(int x,int y)const{
	DrawBox(x,y,x+wholeCommentWidth,y+wholeCommentHeight,GetColor(64,128,192),TRUE);
	DrawBox(x,y,x+wholeCommentWidth,y+wholeCommentHeight,GetColor(128,192,255),FALSE);
	DrawStringNewLineToHandle(x+5,y+5,wholeCommentWidth-10,wholeCommentHeight-10,m_wholeComment.c_str(),GetColor(255,255,255),m_sentenceFont,2);
}
