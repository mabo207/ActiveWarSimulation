#ifndef DEF_SUBMISSIONRULEBASE_H
#define DEF_SUBMISSIONRULEBASE_H

#include<string>
#include<memory>
#include<vector>

#include"LogElement.h"
#include"LatticeBattleField.h"
#include"SubmissionEvaluation.h"

struct BattleSceneData;
class Unit;

//サブミッション評価時のルールを表現するための基底クラス
class SubmissionRuleBase{
public:
	struct InAdvanceCalculateData{
		//事前計算した結果を格納するクラス、継承して用いる
	};

	virtual ~SubmissionRuleBase()=default;
	SubmissionEvaluation RubricEvaluate(const BattleSceneData * const battleData)const;//１つの行動についてルーブリック評価をする
	virtual SubmissionEvaluation RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const=0;
	virtual SubmissionEvaluation InAdvanceDataEvaluate(const std::shared_ptr<InAdvanceCalculateData> &,const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
		//事前計算データを用いて高速評価する関数
		//デフォルトでは、通常評価を行うようになっている
		return RubricEvaluate(field,stageSize,evaluateLog);
	}
	virtual std::string GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const=0;//最も多かったルーブリック評価について、総括的になコメントを返す
	virtual std::string GetWholeLookBackActionEmpty()const=0;//ルールに該当した行動が何もなかった時の総括的評価の文字列を返す
	virtual std::string GetSubmissionExplanation()const=0;//サブミッションの説明文を返す
	virtual bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const=0;//評価を行うかどうか
	virtual std::string GetReason(SubmissionEvaluation rubric)const=0;//ルーブリック評価の理由の文章を返す

protected:
	SubmissionRuleBase()=default;
	//よく評価に用いられる情報を計算する関数、Unit *でなくUnitLogDataを用いて計算をしないといけない
	float CalculateRouteDistance(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;//operatedUnitからaimedUnitまでの経路距離を計算する
	bool JudgeAttackable(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;//operatedUnitがaimedUnitを次の行動で攻撃できるかどうか判定する
	std::vector<bool> JudgeAttackableList(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const std::vector<LogElement::UnitLogData> &aimedUnitList)const;//aimedUnitが複数にした拡張版
	std::vector<bool> JudgeAttackableList(const std::shared_ptr<LatticeBattleField> &latticeField,const LogElement::UnitLogData operatedUnit,const std::vector<LogElement::UnitLogData> &aimedUnitList)const;//格子点の侵入可否情報だけ呼び出し側で自由に決められる。battleDataも渡す必要がなくなる。
	//BattleSceneDataを用いたやつ
	float CalculateRouteDistance(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;//operatedUnitからaimedUnitまでの経路距離を計算する
	bool JudgeAttackable(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;//operatedUnitがaimedUnitを次の行動で攻撃できるかどうか判定する
	std::vector<bool> JudgeAttackableList(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const std::vector<LogElement::UnitLogData> &aimedUnitList)const;//aimedUnitが複数にした拡張版
};

#endif // !DEF_SUBMISSIONRULEBASE_H
