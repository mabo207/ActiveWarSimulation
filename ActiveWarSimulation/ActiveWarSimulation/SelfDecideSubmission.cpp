#include"DxLib.h"
#include"SelfDecideSubmission.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"ToolsLib.h"
#include"GraphicControl.h"

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
	:m_rubricList()
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
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_archer);
}

bool SelfDecideSubmission::JudgeDrawRubric()const{
	if(!m_rubricList.empty()){
		const int evaluate=m_rubricList.back();
		return evaluate>=0;
	}
	return false;
}

void SelfDecideSubmission::RubricEvaluate(const BattleSceneData * const battleData){
	//- 例外処理
	//	- 攻撃しない(-1)
	//- 評価
	//	0. 直線距離が、敵ユニット攻撃射程以下(敵の射程が自分の射程より長い場合は無条件で達成するものとする)
	//	1. ルート距離が、自ユニット攻撃射程以下
	//	2. ルート距離が、敵の移動距離以下
	//	3. 2以上の評価
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(battleData->m_scoreObserver->GetLatestLog());
	int evaluate;
	if(!attackLog){
		//ログがAttackLogでない場合は「攻撃をしなかった」と判断できる
		evaluate=-1;
	} else{
		//距離に関する評価をする
		//直線距離を求める
		const float directDistance=(attackLog->GetOperateUnitData().pos-attackLog->GetAimedUnitData().pos).size();
		//被弾ユニット→行動ユニットへのルート距離を求める
		std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);
		const Unit *operatedUnit=attackLog->GetOperateUnitData().punit;
		const Unit *aimedUnit=attackLog->GetAimedUnit();
		for(const LogElement::UnitLogData &logData:attackLog->m_unitDataList){
			//ユニットの格子点追加(被弾ユニットが動く事を想定している)
			if(logData.punit!=operatedUnit && logData.punit!=aimedUnit){
				latticeField->BecomeImpassibleLattice(logData.punit,aimedUnit->GetBattleStatus().team);
			}
		}
		std::vector<float> distVec=latticeField->CalculateRouteDistance(aimedUnit->getPos(),{operatedUnit->getPos()});
		const float routeDistance=distVec.front();
		//評価(高い方から判定していく)
		if(routeDistance>=attackLog->GetAimedUnit()->GetMaxMoveDistance()){
			evaluate=3;
		} else if(routeDistance>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength()){
			evaluate=2;
		} else if(directDistance>=attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()
			|| attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength())
		{
			evaluate=1;
		} else{
			evaluate=0;
		}
	}
	//評価の蓄積をして処理を終了
	m_rubricList.push_back(evaluate);
}

void SelfDecideSubmission::WholeLookBack(){
	if(m_rubricList.empty()){
		//1回も射手で攻撃していない時
		m_wholeComment="もっと射手で攻撃してみよう！";
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
	switch(mostFrequent.first){
	case(-1):
		//攻撃していない
		m_wholeComment="もっと射手で攻撃してみよう！";
		break;
	case(0):
		//敵ユニットの攻撃射程内からの攻撃
		m_wholeComment="敵の射程外から攻撃する事を意識してみると良いよ！";
		break;
	case(1):
		//ルート距離が攻撃射程以下
		m_wholeComment="弓で攻撃する時は、障害物や味方越しに攻撃してみよう！";
		break;
	case(2):
		//ルート距離が敵の移動距離以下
		m_wholeComment="敵が次の行動でどこまで行けるかを考えて、敵の攻撃を受けないような位置で攻撃することを意識してみよう！";
		break;
	case(3):
		//これ以上の評価
		m_wholeComment="言うことなしです！障害物や味方をうまく使って、安全な場所から攻撃できています！";
		break;
	}
}

void SelfDecideSubmission::DrawSubmission(int x,int y)const{
	const std::string submissionStr="地形や味方キャラの壁を利用して、\n安全地帯から攻撃してみよう！";
	DrawBox(x,y,x+s_submissionWidth,y+s_submissionHeight,GetColor(64,192,64),TRUE);
	DrawBox(x,y,x+s_submissionWidth,y+s_submissionHeight,GetColor(192,255,192),FALSE);
	DrawStringNewLineToHandle(x+5,y+5,s_submissionWidth-10,s_submissionHeight-10,submissionStr.c_str(),GetColor(255,255,255),m_sentenceFont,2);
}

void SelfDecideSubmission::DrawRubric(int centerX,int centerY)const{
	if(!m_rubricList.empty()){
		//ルーブリック評価の文言を定義
		const int evaluate=m_rubricList.back();
		std::string rubricStr;
		const auto it=m_rubricStrMap.find(evaluate);
		if(it!=m_rubricStrMap.end()){
			rubricStr=it->second;
		}
		unsigned int edgeColor;
		switch(evaluate){
		case(-1):
			//攻撃してないときは描画しない
			return;
		case(0):
			//悪い
			edgeColor=GetColor(128,0,196);
			break;
		case(1):
			//悪い
			edgeColor=GetColor(96,96,196);
			break;
		case(2):
			//微妙
			edgeColor=GetColor(128,128,196);
			break;
		case(3):
			//完璧
			edgeColor=GetColor(196,196,64);
			break;
		}
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
