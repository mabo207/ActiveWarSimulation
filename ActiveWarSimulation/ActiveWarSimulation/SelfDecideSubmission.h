#ifndef DEF_SELFDECIDESUBMISSION_H
#define DEF_SELFDECIDESUBMISSION_H

#include<memory>
#include<vector>
#include<string>

struct BattleSceneData;

//自己設定サブミッション
//ひとまず「安全地帯から弓で攻撃する」をここに実装する、汎用化・拡張は後で行う
class SelfDecideSubmission{
public:
	SelfDecideSubmission()=default;
	~SelfDecideSubmission()=default;
	//観察条件
	bool JudgeEvaluatedOrder(const std::shared_ptr<BattleSceneData> &battleData)const;
	//ルーブリック評価を行い蓄積する
	void RubricEvaluate(const std::shared_ptr<BattleSceneData> &battleData);
	//総括的振り返りをする
	void WholeLookBack();
	//ミッション内容を描画する
	void DrawSubmission(int x,int y,int font)const;
	//トップのルーブリック評価の結果を描画する
	void DrawRubric(int x,int y,int font)const;
	//総括的振り返りの内容を描画する
	void DrawWholeLookBack(int x,int y,int font)const;

private:
	std::vector<int> m_rubricList;//ルーブリック評価一覧
	std::string m_wholeComment;//総括的振り返りにおけるコメント
};

#endif // !DEF_SELFDECIDESUBMISSION_H
