#include"TutorialBattleSceneData.h"
#include"Circle.h"

//----------------TutorialBattleSceneData::MoveTutorial----------------------
TutorialBattleSceneData::MoveTutorial::MoveTutorial(std::shared_ptr<Shape> moveTutorialArea)
	:TutorialBase(TutorialKind::e_move)
	,m_moveTutorialArea(moveTutorialArea)
{}

TutorialBattleSceneData::MoveTutorial::~MoveTutorial(){}

//----------------TutorialBattleSceneData::AttackTutorial----------------------
TutorialBattleSceneData::AttackTutorial::AttackTutorial(const Unit *targetUnit)
	:TutorialBase(TutorialKind::e_normalAttack)
	,m_targetUnit(targetUnit)
{}

TutorialBattleSceneData::AttackTutorial::~AttackTutorial(){}

//----------------TutorialBattleSceneData::WaitTutorial----------------------
TutorialBattleSceneData::WaitTutorial::WaitTutorial()
	:TutorialBase(TutorialKind::e_wait)
{}

TutorialBattleSceneData::WaitTutorial::~WaitTutorial(){}

//----------------TutorialBattleSceneData-----------------------
TutorialBattleSceneData::TutorialBattleSceneData()
	:BattleSceneData("tutorial",PlayMode::e_tutorial)
{
	//チュートリアルデータの読み込み

	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new MoveTutorial(std::shared_ptr<Shape>(new Circle(Vector2D(395.0f,425.0f),45.0f,Shape::Fix::e_ignore)))));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new AttackTutorial(m_unitList[1])));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new WaitTutorial()));
}

TutorialBattleSceneData::~TutorialBattleSceneData()
{
}
