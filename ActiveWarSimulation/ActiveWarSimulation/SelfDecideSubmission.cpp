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
const int SelfDecideSubmission::s_submissionWidth=700;
const int SelfDecideSubmission::s_submissionHeight=90;
const int SelfDecideSubmission::s_reasonWidth=700;
const int SelfDecideSubmission::s_reasonHeight=90;

SelfDecideSubmission::SelfDecideSubmission()
	:m_rule(nullptr)
	,m_rubricList()
	,m_wholeComment()
	,m_rubricFrequencyMap()
	,m_sentenceFont(CreateFontToHandleEX("メイリオ",24,2,DX_FONTTYPE_NORMAL))
	,m_rubricFont(CreateFontToHandleEX("メイリオ",20,2,DX_FONTTYPE_EDGE,-1,2))
{}

SelfDecideSubmission::~SelfDecideSubmission(){
	DeleteFontToHandleEX(m_sentenceFont);
	DeleteFontToHandleEX(m_rubricFont);
}

bool SelfDecideSubmission::JudgeEvaluatedOrder(const BattleSceneData * const battleData)const{
	if(m_rule){
		return m_rule->JudgeEvaluateOrder(battleData);
	} else{
		//m_ruleが存在しない時は評価が行われないのと同義なのでfalse
		return false;
	}
}

bool SelfDecideSubmission::JudgeDrawRubric()const{
	if(!m_rubricList.empty() && m_rule){
		//評価が存在しないまたはm_ruleが存在しない時は描画しない
		const SubmissionEvaluation evaluate=m_rubricList.back().first;
		return evaluate!=SubmissionEvaluation::e_noevaluation;
	}
	return false;
}

void SelfDecideSubmission::RubricEvaluate(const BattleSceneData * const battleData){
	//評価の蓄積
	if(m_rule){
		m_rubricList.push_back(std::make_pair(m_rule->RubricEvaluate(battleData),battleData->m_scoreObserver->GetLatestLog()));
	}
}

void SelfDecideSubmission::WholeLookBack(){
	if(m_rule){
		//Ruleクラスに移譲
		if(m_rubricList.empty()){
			//1回も評価対象となる行動をしていない時
			m_wholeComment=m_rule->GetWholeLookBackActionEmpty();
			return;
		}
		//最頻値を求める
		for(const std::pair<SubmissionEvaluation,std::shared_ptr<const LogElement>> &pair:m_rubricList){
			const SubmissionEvaluation rubric=pair.first;
			std::map<SubmissionEvaluation,size_t>::iterator it=m_rubricFrequencyMap.find(rubric);
			if(it==m_rubricFrequencyMap.end()){
				//まだrubricが1回も生じていない場合は、m_rubricFrequencyMapに追加
				m_rubricFrequencyMap.insert(std::make_pair(rubric,1));
			} else{
				//既に見つかっている場合は、回数を1増やす
				it->second++;
			}
		}
		std::pair<SubmissionEvaluation,size_t> mostFrequent=*m_rubricFrequencyMap.begin();
		for(const std::pair<SubmissionEvaluation,size_t> &pair:m_rubricFrequencyMap){
			if(mostFrequent.first==SubmissionEvaluation::e_noevaluation){
				//mostFrequentが「攻撃していない」である場合は、必ず上書きする
				mostFrequent=pair;
			} else if(pair.second>mostFrequent.second && pair.first!=SubmissionEvaluation::e_noevaluation){
				//「攻撃していない」以外の頻度が大きいものに更新
				mostFrequent=pair;
			}
		}
		//最頻値のルーブリックに応じてコメントを格納
		m_wholeComment=m_rule->GetWholeLookBack(mostFrequent.first);
	}
}

void SelfDecideSubmission::DrawSubmission(int x,int y)const{
	if(m_rule){
		//文字列指定はRuleに移譲
		const std::string submissionStr=m_rule->GetSubmissionExplanation();
		DrawBox(x,y,x+s_submissionWidth,y+s_submissionHeight,GetColor(64,192,64),TRUE);
		DrawBox(x,y,x+s_submissionWidth,y+s_submissionHeight,GetColor(192,255,192),FALSE);
		DrawStringNewLineToHandle(x+5,y+5,s_submissionWidth-10,s_submissionHeight-10,submissionStr.c_str(),GetColor(255,255,255),m_sentenceFont,2);
	}
}

void SelfDecideSubmission::DrawRubric(int centerX,int centerY)const{
	if(!m_rubricList.empty() && m_rule){
		//ルーブリック評価の文言を定義(Ruleに移譲)
		const SubmissionEvaluation evaluate=m_rubricList.back().first;
		const std::string rubricStr=evaluate.GetString();
		const unsigned int edgeColor=evaluate.Color();
		//描画
		DrawStringCenterBaseToHandle(centerX,centerY,rubricStr.c_str(),GetColor(255,255,255),m_rubricFont,true,edgeColor);
	}
}

void SelfDecideSubmission::DrawReason(int x,int y)const{
	if(!m_rubricList.empty() && m_rule){
		//描画内容の決定
		const std::string str=m_rule->GetReason(m_rubricList.back().first);
		//下地
		DrawBox(x,y,x+s_reasonWidth,y+s_reasonHeight,GetColor(64,128,192),TRUE);
		DrawBox(x,y,x+s_reasonWidth,y+s_reasonHeight,GetColor(128,192,255),FALSE);
		//内容の描画
		DrawStringNewLineToHandle(x+5,y+5,s_reasonWidth-10,s_reasonHeight-10,str.c_str(),GetColor(255,255,255),m_sentenceFont,2);
	}
}

void SelfDecideSubmission::DrawWholeLookBack(int x,int y)const{
	if(m_rule){
		//下地
		DrawBox(x,y,x+wholeCommentWidth,y+wholeCommentHeight,GetColor(64,128,192),TRUE);
		DrawBox(x,y,x+wholeCommentWidth,y+wholeCommentHeight,GetColor(128,192,255),FALSE);
		//評価の一覧を描画
		int strY=y+5;
		const int fontSize=GetFontSizeToHandle(m_sentenceFont);
		for(const auto &pair:m_rubricFrequencyMap){
			const std::string rubricStr=pair.first.GetString();
			if(!rubricStr.empty()){
				DrawStringToHandle(x+5,strY,rubricStr.c_str(),GetColor(255,255,255),m_sentenceFont);//難易度名
				DrawStringToHandle(x+wholeCommentWidth-150,strY,(":   ×"+to_string_0d(pair.second,2)).c_str(),GetColor(255,255,255),m_sentenceFont);//回数の描画
				strY+=fontSize+2;
			}
		}
		//包括的コメント
		DrawStringNewLineToHandle(x+5,strY+10,wholeCommentWidth-10,y+wholeCommentHeight-5-strY,m_wholeComment.c_str(),GetColor(255,255,255),m_sentenceFont,2);
	}
}

void SelfDecideSubmission::InitRubric(const std::shared_ptr<SubmissionRuleBase> &rule){
	m_rule=rule;
}

WholeReflectionInfo SelfDecideSubmission::GetReflectionInfo()const{
	std::pair<SubmissionEvaluation,std::shared_ptr<const LogElement>> goodLog,badLog;
	if(!m_rubricList.empty()){
		//暫定的に評価に-1を格納
		goodLog.first=SubmissionEvaluation::e_noevaluation;
		badLog.first=SubmissionEvaluation::e_noevaluation;
		//探索
		for(const std::pair<SubmissionEvaluation,std::shared_ptr<const LogElement>> &log:m_rubricList){
			//評価が良いものと悪いものを探していく、-1評価は必ず更新する
			if((goodLog.first<log.first || badLog.first==SubmissionEvaluation::e_noevaluation) && log.first!=SubmissionEvaluation::e_noevaluation){
				goodLog=log;
			}
			if((badLog.first>log.first || badLog.first==SubmissionEvaluation::e_noevaluation) && log.first!=SubmissionEvaluation::e_noevaluation){
				badLog=log;
			}
		}
	}
	return WholeReflectionInfo(goodLog,badLog);
}
