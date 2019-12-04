#ifndef DEF_PROTECTFRIEND_H
#define DEF_PROTECTFRIEND_H

#include"SubmissionRuleBase.h"

//味方を守るように配置できているか評価する
class ProtectFriend:public SubmissionRuleBase{
public:
	ProtectFriend()=default;
	~ProtectFriend()=default;
	SubmissionEvaluation InAdvanceDataEvaluate(const std::shared_ptr<InAdvanceCalculateDataBase> &,const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const;//１つの行動についてルーブリック評価をする
	std::string GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const;//最も多かったルーブリック評価について、総括的になコメントを返す
	std::string GetWholeLookBackActionEmpty()const;//ルールに該当した行動が何もなかった時の総括的評価の文字列を返す
	std::string GetSubmissionExplanation()const;//サブミッションの説明文を返す
	bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const;//評価を行うかどうか
	std::string GetReason(SubmissionEvaluation rubric)const;//ルーブリック評価の理由の説明文を返す
};

#endif // !DEF_PROTECTFRIEND_H
