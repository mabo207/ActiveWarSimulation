#ifndef DEF_ARMERPOSITION_H
#define DEF_ARMERPOSITION_H

#include"SubmissionRuleBase.h"

//重装兵の待機ポジションについての評価ルール
class ArmerPosition:public SubmissionRuleBase{
public:
	ArmerPosition()=default;
	~ArmerPosition()=default;
	int RubricEvaluate(const BattleSceneData * const battleData)const;//１つの行動についてルーブリック評価をする
	std::pair<std::string,unsigned int> GetRubricStringInfo(int rubric)const;//ルーブリック評価に対する文字列を描画するための情報を返す（文字列＆文字の縁の色）
	std::string GetWholeLookBack(int mostFrequentEvaluate)const;//最も多かったルーブリック評価について、総括的になコメントを返す
	std::string GetWholeLookBackActionEmpty()const;//ルールに該当した行動が何もなかった時の総括的評価の文字列を返す
	std::string GetSubmissionExplanation()const;//サブミッションの説明文を返す
	bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const;//評価を行うかどうか
};

#endif // !DEF_ARMERPOSITION_H
