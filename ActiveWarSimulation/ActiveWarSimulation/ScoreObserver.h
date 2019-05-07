#ifndef DEF_SCOREOBSERVER_H
#define DEF_SCOREOBSERVER_H

#include<vector>
#include<memory>
#include<set>
#include"LogElement.h"

//"BattleSceneData.h"と"ScoreSystem.h"は循環参照になってしまうので宣言
struct BattleSceneData;

class ScoreObserver{
public:
	//各場面に応じたスコアの更新処理
	void InitUpdate(const BattleSceneData * const battleData);
	void FinishUpdate(const BattleSceneData * const battleData);
	void AttackUpdate(const BattleSceneData * const battleData,const Unit * const aimedUnit);
	void ResearchUpdate();
	void WaitUpdate(const BattleSceneData * const battleData);
	void CancelUpdate();
	//スコアの結果出力処理
	std::string GetScoreExplain()const;
	//コンストラクタ等
	ScoreObserver();
	virtual ~ScoreObserver();

private:

	std::vector<std::shared_ptr<LogElement>> m_logList;
	size_t m_researchCount;
	size_t m_cancelCount;
};

#endif // !DEF_SCOREOBSERVER_H