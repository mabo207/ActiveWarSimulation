#ifndef DEF_SUBMISSIONRULEBASE_H
#define DEF_SUBMISSIONRULEBASE_H

#include<string>

struct BattleSceneData;

//サブミッション評価時のルールを表現するための基底クラス
class SubmissionRuleBase{
public:
	virtual ~SubmissionRuleBase()=default;
	virtual int RubricEvaluate(const BattleSceneData * const battleData)const=0;//１つの行動についてルーブリック評価をする
	virtual std::pair<std::string,unsigned int> GetRubricStringInfo(int rubric)const=0;//ルーブリック評価に対する文字列を描画するための情報を返す（文字列＆文字の縁の色）
	virtual std::string GetWholeLookBack(int mostFrequentEvaluate)const=0;//最も多かったルーブリック評価について、総括的になコメントを返す
	virtual std::string GetWholeLookBackActionEmpty()const=0;//ルールに該当した行動が何もなかった時の総括的評価の文字列を返す
	virtual std::string GetSubmissionExplanation()const=0;//サブミッションの説明文を返す
	virtual bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const=0;//評価を行うかどうか

protected:
	SubmissionRuleBase()=default;
};

#endif // !DEF_SUBMISSIONRULEBASE_H
