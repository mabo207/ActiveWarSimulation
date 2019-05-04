#include"ScoreObserver.h"
#include"WaitLog.h"

void ScoreObserver::AttackUpdate(const std::shared_ptr<const BattleSceneData> battleData,const Unit * const aimedUnit){
	m_logList.push_back(std::shared_ptr<LogElement>(new WaitLog(battleData)));
}

void ScoreObserver::ResearchUpdate(){
	m_researchCount++;
}

void ScoreObserver::WaitUpdate(const std::shared_ptr<const BattleSceneData> battleData){
	m_logList.push_back(std::shared_ptr<LogElement>(new WaitLog(battleData)));
}

void ScoreObserver::CancelUpdate(){
	m_cancelCount++;
}

void ScoreObserver::ClearUpdate(const std::shared_ptr<const BattleSceneData> battleData){

}

ScoreObserver::ScoreObserver()
	:m_researchCount(0)
	,m_cancelCount(0)
	,m_logList()
{}

ScoreObserver::~ScoreObserver(){}