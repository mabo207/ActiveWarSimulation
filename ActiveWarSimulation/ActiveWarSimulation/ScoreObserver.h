#ifndef DEF_SCOREOBSERVER_H
#define DEF_SCOREOBSERVER_H

#include<vector>
#include<memory>
#include<set>
#include"LogElement.h"

//"BattleSceneData.h"と"ScoreSystem.h"と"LogElement.h"は循環参照
struct BattleSceneData;
class LogElement;

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
	void AttackUpdate(const std::shared_ptr<const BattleSceneData> battleData,const Unit * const aimedUnit);
	void ResearchUpdate();
	void WaitUpdate(const std::shared_ptr<const BattleSceneData> battleData);
	void CancelUpdate();
	void ClearUpdate(const std::shared_ptr<const BattleSceneData> battleData);

	ScoreObserver();
	virtual ~ScoreObserver();

private:

	std::vector<std::shared_ptr<LogElement>> m_logList;
	size_t m_researchCount;
	size_t m_cancelCount;
};

#endif // !DEF_SCOREOBSERVER_H