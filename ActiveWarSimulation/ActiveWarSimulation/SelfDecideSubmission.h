#ifndef DEF_SELFDECIDESUBMISSION_H
#define DEF_SELFDECIDESUBMISSION_H

#include<memory>
#include<vector>
#include<string>
#include"SubmissionRuleBase.h"
#include<map>

struct BattleSceneData;

//自己設定サブミッション
//ひとまず「安全地帯から弓で攻撃する」をここに実装する、汎用化・拡張は後で行う
class SelfDecideSubmission{
public:
	SelfDecideSubmission();
	~SelfDecideSubmission();
	//観察条件
	bool JudgeEvaluatedOrder(const BattleSceneData * const battleData)const;
	//ルーブリック描画条件
	bool JudgeDrawRubric()const;
	//ルーブリック評価を行い蓄積する
	void RubricEvaluate(const BattleSceneData * const battleData);
	//総括的振り返りをする
	void WholeLookBack();
	//ミッション内容を描画する
	void DrawSubmission(int x,int y)const;
	//トップのルーブリック評価の結果を描画する
	void DrawRubric(int centerX,int centerY)const;
	//トップのルーブリック評価について、結果の理由を描画する
	void DrawReason(int x,int y)const;
	//総括的振り返りの内容を描画する
	void DrawWholeLookBack(int x,int y)const;
	//ルール初期化
	void InitRubric(const std::shared_ptr<SubmissionRuleBase> &rule);
	//リフレクションするログを取得する
	std::shared_ptr<const LogElement> GetReflectionLog()const{
		return m_reflectionLog;
	}

	//サブミッション描画の領域の大きさ
	static const int s_submissionWidth;
	static const int s_submissionHeight;
	//評価理由描画の領域の大きさ
	static const int s_reasonWidth;
	static const int s_reasonHeight;

private:
	std::shared_ptr<SubmissionRuleBase> m_rule;//ルーブリック評価のルール
	std::vector<int> m_rubricList;//ルーブリック評価一覧
	std::string m_wholeComment;//総括的振り返りにおけるコメント
	std::map<int,size_t> m_rubricFrequencyMap;//戦闘終了時にルーブリック評価の回数の一覧を格納する
	std::shared_ptr<const LogElement> m_reflectionLog;//リフレクションを行う場面

	const int m_sentenceFont;//文章を描画するためのフォント
	const int m_rubricFont;//ルーブリック評価を描画するためのフォント
};

#endif // !DEF_SELFDECIDESUBMISSION_H
