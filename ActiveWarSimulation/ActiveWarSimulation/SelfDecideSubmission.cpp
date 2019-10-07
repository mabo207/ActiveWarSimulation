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

SelfDecideSubmission::SelfDecideSubmission(const std::shared_ptr<SubmissionRuleBase> &rule)
	:m_rule(rule)
	,m_rubricList()
	,m_wholeComment()
	,m_rubricStrMap{std::make_pair(0,"Worst"),std::make_pair(1,"Bad"),std::make_pair(2,"Not good"),std::make_pair(3,"Good")}
	,m_rubricFrequencyMap()
	,m_sentenceFont(CreateFontToHandleEX("メイリオ",24,2,DX_FONTTYPE_NORMAL))
	,m_rubricFont(CreateFontToHandleEX("メイリオ",20,2,DX_FONTTYPE_EDGE,-1,2))
{}

SelfDecideSubmission::~SelfDecideSubmission(){
	DeleteFontToHandleEX(m_sentenceFont);
	DeleteFontToHandleEX(m_rubricFont);
}

bool SelfDecideSubmission::JudgeEvaluatedOrder(const BattleSceneData * const battleData)const{
	return m_rule->JudgeEvaluateOrder(battleData);
}

bool SelfDecideSubmission::JudgeDrawRubric()const{
	if(!m_rubricList.empty()){
		const int evaluate=m_rubricList.back();
		return evaluate>=0;
	}
	return false;
}

void SelfDecideSubmission::RubricEvaluate(const BattleSceneData * const battleData){
	//評価の蓄積
	m_rubricList.push_back(m_rule->RubricEvaluate(battleData));
}

void SelfDecideSubmission::WholeLookBack(){
	//Ruleクラスに移譲
	if(m_rubricList.empty()){
		//1回も評価対象となる行動をしていない時
		m_wholeComment=m_rule->GetWholeLookBackActionEmpty();
		return;
	}
	//最頻値を求める
	for(const int &rubric:m_rubricList){
		std::map<int,size_t>::iterator it=m_rubricFrequencyMap.find(rubric);
		if(it==m_rubricFrequencyMap.end()){
			//まだrubricが1回も生じていない場合は、m_rubricFrequencyMapに追加
			m_rubricFrequencyMap.insert(std::make_pair(rubric,1));
		} else{
			//既に見つかっている場合は、回数を1増やす
			it->second++;
		}
	}
	std::pair<int,size_t> mostFrequent=*m_rubricFrequencyMap.begin();
	for(const std::pair<int,size_t> &pair:m_rubricFrequencyMap){
		if(mostFrequent.first==-1){
			//mostFrequentが「攻撃していない」である場合は、必ず上書きする
			mostFrequent=pair;
		} else if(pair.second>mostFrequent.second && pair.first!=-1){
			//「攻撃していない」以外の頻度が大きいものに更新
			mostFrequent=pair;
		}
	}
	//最頻値のルーブリックに応じてコメントを格納
	m_wholeComment=m_rule->GetWholeLookBack(mostFrequent.first);
}

void SelfDecideSubmission::DrawSubmission(int x,int y)const{
	//文字列指定はRuleに移譲
	const std::string submissionStr=m_rule->GetSubmissionExplanation();
	DrawBox(x,y,x+s_submissionWidth,y+s_submissionHeight,GetColor(64,192,64),TRUE);
	DrawBox(x,y,x+s_submissionWidth,y+s_submissionHeight,GetColor(192,255,192),FALSE);
	DrawStringNewLineToHandle(x+5,y+5,s_submissionWidth-10,s_submissionHeight-10,submissionStr.c_str(),GetColor(255,255,255),m_sentenceFont,2);
}

void SelfDecideSubmission::DrawRubric(int centerX,int centerY)const{
	if(!m_rubricList.empty()){
		//ルーブリック評価の文言を定義(Ruleに移譲)
		std::pair<std::string,unsigned int> pair=m_rule->GetRubricStringInfo(m_rubricList.back());
		std::string rubricStr=pair.first;
		unsigned int edgeColor=pair.second;
		//描画
		DrawStringCenterBaseToHandle(centerX,centerY,rubricStr.c_str(),GetColor(255,255,255),m_rubricFont,true,edgeColor);
	}
}

void SelfDecideSubmission::DrawReason(int x,int y)const{
	if(!m_rubricList.empty()){
		//描画内容の決定
		const int evaluate=m_rubricList.back();
		std::string str;
		switch(evaluate){
		case(-1):
			//描画を行わない
			return;
		case(0):
			str="敵がその場で攻撃できるくらいに近くで攻撃しちゃってるよ！";
			break;
		case(1):
			str="障害物が周りにないから攻撃した敵の反撃に合いやすそうじゃない？";
			break;
		case(2):
			str="障害物越しに攻撃できてるけど、案外敵は回り込んで攻撃できそう。";
			break;
		case(3):
			str="安全地帯からの攻撃、とっても良い感じ！！";
			break;
		}
		//下地
		DrawBox(x,y,x+s_reasonWidth,y+s_reasonHeight,GetColor(64,128,192),TRUE);
		DrawBox(x,y,x+s_reasonWidth,y+s_reasonHeight,GetColor(128,192,255),FALSE);
		//内容の描画
		DrawStringNewLineToHandle(x+5,y+5,s_reasonWidth-10,s_reasonHeight-10,str.c_str(),GetColor(255,255,255),m_sentenceFont,2);
	}
}

void SelfDecideSubmission::DrawWholeLookBack(int x,int y)const{
	//下地
	DrawBox(x,y,x+wholeCommentWidth,y+wholeCommentHeight,GetColor(64,128,192),TRUE);
	DrawBox(x,y,x+wholeCommentWidth,y+wholeCommentHeight,GetColor(128,192,255),FALSE);
	//評価の一覧を描画
	int strY=y+5;
	const int fontSize=GetFontSizeToHandle(m_sentenceFont);
	for(const auto &pair:m_rubricFrequencyMap){
		const auto rubricStrIt=m_rubricStrMap.find(pair.first);
		if(rubricStrIt!=m_rubricStrMap.end()){
			DrawStringToHandle(x+5,strY,rubricStrIt->second.c_str(),GetColor(255,255,255),m_sentenceFont);//難易度名
			DrawStringToHandle(x+wholeCommentWidth-150,strY,(":   ×"+to_string_0d(pair.second,2)).c_str(),GetColor(255,255,255),m_sentenceFont);//回数の描画
			strY+=fontSize+2;
		}
	}
	//包括的コメント
	DrawStringNewLineToHandle(x+5,strY+10,wholeCommentWidth-10,y+wholeCommentHeight-5-strY,m_wholeComment.c_str(),GetColor(255,255,255),m_sentenceFont,2);
}
