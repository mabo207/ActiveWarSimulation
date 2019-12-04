#ifndef DEF_ARCHERATTACKDISTANCE_H
#define DEF_ARCHERATTACKDISTANCE_H

#include"SubmissionRuleBase.h"
#include<array>

class ArcherAttackDistance:public SubmissionRuleBase{
public:
	ArcherAttackDistance()=default;
	~ArcherAttackDistance()=default;
	//ルーブリック評価関係
	std::shared_ptr<InAdvanceCalculateDataBase> CalculateInAdvanceData(
		const std::vector<BattleObject *> &field
		,const Vector2D stageSize
		,const std::shared_ptr<const LogElement> &evaluateLog)const;
	SubmissionEvaluation InAdvanceDataEvaluate(
		const std::shared_ptr<InAdvanceCalculateDataBase> &inAdvanceData
		,const std::vector<BattleObject *> &field
		,const Vector2D stageSize
		,const std::shared_ptr<const LogElement> &evaluateLog)const;
	//その他の関数
	std::string GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const;//最も多かったルーブリック評価について、総括的になコメントを返す
	std::string GetWholeLookBackActionEmpty()const;//ルールに該当した行動が何もなかった時の総括的評価の文字列を返す
	std::string GetSubmissionExplanation()const;//サブミッションの説明文を返す
	bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const;//評価を行うかどうか
	std::string GetReason(SubmissionEvaluation rubric)const;//ルーブリック評価の理由の説明文を返す

	static const std::array<SubmissionEvaluation,4> s_evaluate;//「評価なし」以外で使われる評価一覧
};

#endif // !DEF_ARCHERATTACKDISTANCE_H
