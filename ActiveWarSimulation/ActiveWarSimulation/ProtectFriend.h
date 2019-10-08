#ifndef DEF_PROTECTFRIEND_H
#define DEF_PROTECTFRIEND_H

#include"SubmissionRuleBase.h"

//味方を守るように配置できているか評価する
class ProtectFriend:public SubmissionRuleBase{
public:
	ProtectFriend()=default;
	~ProtectFriend()=default;
	int RubricEvaluate(const BattleSceneData * const battleData)const;//１つの行動についてルーブリック評価をする
	std::pair<std::string,unsigned int> GetRubricStringInfo(int rubric)const;//ルーブリック評価に対する文字列を描画するための情報を返す（文字列＆文字の縁の色）
	std::string GetWholeLookBack(int mostFrequentEvaluate)const;//最も多かったルーブリック評価について、総括的になコメントを返す
	std::string GetWholeLookBackActionEmpty()const;//ルールに該当した行動が何もなかった時の総括的評価の文字列を返す
	std::string GetSubmissionExplanation()const;//サブミッションの説明文を返す
	bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const;//評価を行うかどうか
	std::string GetReason(int rubric)const;//ルーブリック評価の理由の説明文を返す
};

#endif // !DEF_PROTECTFRIEND_H
