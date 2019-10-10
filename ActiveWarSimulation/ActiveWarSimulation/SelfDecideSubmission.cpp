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
const int SelfDecideSubmission::s_reasonWidth=600;
const int SelfDecideSubmission::s_reasonHeight=100;

SelfDecideSubmission::SelfDecideSubmission()
	:m_rule(nullptr)
	,m_rubricList()
	,m_wholeComment()
	,m_rubricFrequencyMap()
	,m_sentenceFont(CreateFontToHandleEX("���C���I",24,2,DX_FONTTYPE_NORMAL))
	,m_rubricFont(CreateFontToHandleEX("���C���I",20,2,DX_FONTTYPE_EDGE,-1,2))
{}

SelfDecideSubmission::~SelfDecideSubmission(){
	DeleteFontToHandleEX(m_sentenceFont);
	DeleteFontToHandleEX(m_rubricFont);
}

bool SelfDecideSubmission::JudgeEvaluatedOrder(const BattleSceneData * const battleData)const{
	if(m_rule){
		return m_rule->JudgeEvaluateOrder(battleData);
	} else{
		//m_rule�����݂��Ȃ����͕]�����s���Ȃ��̂Ɠ��`�Ȃ̂�false
		return false;
	}
}

bool SelfDecideSubmission::JudgeDrawRubric()const{
	if(!m_rubricList.empty() && m_rule){
		//�]�������݂��Ȃ��܂���m_rule�����݂��Ȃ����͕`�悵�Ȃ�
		const int evaluate=m_rubricList.back();
		return evaluate>=0;
	}
	return false;
}

void SelfDecideSubmission::RubricEvaluate(const BattleSceneData * const battleData){
	//�]���̒~��
	if(m_rule){
		m_rubricList.push_back(m_rule->RubricEvaluate(battleData));
	}
}

void SelfDecideSubmission::WholeLookBack(){
	if(m_rule){
		//Rule�N���X�Ɉڏ�
		if(m_rubricList.empty()){
			//1����]���ΏۂƂȂ�s�������Ă��Ȃ���
			m_wholeComment=m_rule->GetWholeLookBackActionEmpty();
			return;
		}
		//�ŕp�l�����߂�
		for(const int &rubric:m_rubricList){
			std::map<int,size_t>::iterator it=m_rubricFrequencyMap.find(rubric);
			if(it==m_rubricFrequencyMap.end()){
				//�܂�rubric��1��������Ă��Ȃ��ꍇ�́Am_rubricFrequencyMap�ɒǉ�
				m_rubricFrequencyMap.insert(std::make_pair(rubric,1));
			} else{
				//���Ɍ������Ă���ꍇ�́A�񐔂�1���₷
				it->second++;
			}
		}
		std::pair<int,size_t> mostFrequent=*m_rubricFrequencyMap.begin();
		for(const std::pair<int,size_t> &pair:m_rubricFrequencyMap){
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
}

void SelfDecideSubmission::DrawSubmission(int x,int y)const{
	if(m_rule){
		//������w���Rule�Ɉڏ�
		const std::string submissionStr=m_rule->GetSubmissionExplanation();
		DrawBox(x,y,x+s_submissionWidth,y+s_submissionHeight,GetColor(64,192,64),TRUE);
		DrawBox(x,y,x+s_submissionWidth,y+s_submissionHeight,GetColor(192,255,192),FALSE);
		DrawStringNewLineToHandle(x+5,y+5,s_submissionWidth-10,s_submissionHeight-10,submissionStr.c_str(),GetColor(255,255,255),m_sentenceFont,2);
	}
}

void SelfDecideSubmission::DrawRubric(int centerX,int centerY)const{
	if(!m_rubricList.empty() && m_rule){
		//���[�u���b�N�]���̕������`(Rule�Ɉڏ�)
		std::pair<std::string,unsigned int> pair=m_rule->GetRubricStringInfo(m_rubricList.back());
		std::string rubricStr=pair.first;
		unsigned int edgeColor=pair.second;
		//�`��
		DrawStringCenterBaseToHandle(centerX,centerY,rubricStr.c_str(),GetColor(255,255,255),m_rubricFont,true,edgeColor);
	}
}

void SelfDecideSubmission::DrawReason(int x,int y)const{
	if(!m_rubricList.empty() && m_rule){
		//�`����e�̌���
		const std::string str=m_rule->GetReason(m_rubricList.back());
		//���n
		DrawBox(x,y,x+s_reasonWidth,y+s_reasonHeight,GetColor(64,128,192),TRUE);
		DrawBox(x,y,x+s_reasonWidth,y+s_reasonHeight,GetColor(128,192,255),FALSE);
		//���e�̕`��
		DrawStringNewLineToHandle(x+5,y+5,s_reasonWidth-10,s_reasonHeight-10,str.c_str(),GetColor(255,255,255),m_sentenceFont,2);
	}
}

void SelfDecideSubmission::DrawWholeLookBack(int x,int y)const{
	if(m_rule){
		//���n
		DrawBox(x,y,x+wholeCommentWidth,y+wholeCommentHeight,GetColor(64,128,192),TRUE);
		DrawBox(x,y,x+wholeCommentWidth,y+wholeCommentHeight,GetColor(128,192,255),FALSE);
		//�]���̈ꗗ��`��
		int strY=y+5;
		const int fontSize=GetFontSizeToHandle(m_sentenceFont);
		for(const auto &pair:m_rubricFrequencyMap){
			const std::string rubricStr=m_rule->GetRubricStringInfo(pair.first).first;
			if(!rubricStr.empty()){
				DrawStringToHandle(x+5,strY,rubricStr.c_str(),GetColor(255,255,255),m_sentenceFont);//��Փx��
				DrawStringToHandle(x+wholeCommentWidth-150,strY,(":   �~"+to_string_0d(pair.second,2)).c_str(),GetColor(255,255,255),m_sentenceFont);//�񐔂̕`��
				strY+=fontSize+2;
			}
		}
		//��I�R�����g
		DrawStringNewLineToHandle(x+5,strY+10,wholeCommentWidth-10,y+wholeCommentHeight-5-strY,m_wholeComment.c_str(),GetColor(255,255,255),m_sentenceFont,2);
	}
}

void SelfDecideSubmission::InitRubric(const std::shared_ptr<SubmissionRuleBase> &rule){
	m_rule=rule;
}
