#ifndef DEF_SCOREOBSERVER_H
#define DEF_SCOREOBSERVER_H

#include<vector>
#include<memory>
#include<set>
#include"BattleSceneData.h"
#include"LogElement.h"

class ScoreObserver{
public:
	//スコア計算の項目
	struct ScoreRule{
		//どの場面で更新するか
		enum class UpdateScene{
			e_attack
			,e_research
			,e_wait
			,e_cancel
			,e_clear
		};
		explicit ScoreRule(const std::set<UpdateScene> &updateSceneSet):m_updateSceneSet(updateSceneSet){}
		virtual ~ScoreRule(){}
		bool JudgeUpdate(UpdateScene updateScene)const{
			return (m_updateSceneSet.find(updateScene)!=m_updateSceneSet.end());
		}
		//この項目はどの場面でスコア更新処理をするか(複数場面を想定)
		const std::set<UpdateScene> m_updateSceneSet;
	};

	//各場面に応じたスコアの更新処理
	void AttackUpdate(const BattleSceneData * const battleData,const Unit * const aimedUnit);
	void ResearchUpdate();
	void WaitUpdate(const BattleSceneData * const battleData);
	void CancelUpdate();
	void ClearUpdate(const BattleSceneData * const battleData);

private:
	ScoreObserver();
	virtual ~ScoreObserver();

	std::vector<std::shared_ptr<LogElement>> m_logList;
};

#endif // !DEF_SCOREOBSERVER_H