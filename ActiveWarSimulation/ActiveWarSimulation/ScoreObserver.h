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
	class LatticeBonusData{
		//格子点関係のボーナスを作成するためのデータと処理
	public:
		LatticeBonusData();
		~LatticeBonusData();
		void InputData(const BattleSceneData * const battleData,const LogElement::UnitLogData &operateUnitData);
		std::pair<std::string,int> GetPlayerBonus()const;
		std::pair<std::string,int> GetEnemyBonus()const;

	private:
		size_t m_playerCount;//プレイヤーキャラの合計行動回数
		double m_totalPlayerRate;//プレイヤーキャラについて「何もなかったら行ける格子点の内実際に行ける格子点の割合」の合計
		size_t m_enemyCount;//敵キャラの合計行動回数
		double m_totalEnemyRate;
	};

	std::vector<std::shared_ptr<LogElement>> m_logList;
	//ログとして残さない些細なデータ
	size_t m_researchCount;
	size_t m_cancelCount;
	//格子点関係についてはログを用いて計算すると処理が長過ぎになるので、Updateのたびにデータを格納する
	LatticeBonusData m_latticeBonusData;
};

#endif // !DEF_SCOREOBSERVER_H