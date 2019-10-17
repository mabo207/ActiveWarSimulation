#include"SubmissionSwitchUnitScene.h"
#include"SelectSubmissionScene.h"
#include"SubmissionPlayerMoveScene.h"

//-------------SubmissionSwitchUnitScene--------------
SubmissionSwitchUnitScene::SubmissionSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:SwitchUnitInitOrderScene(battleSceneData)
{}

SubmissionSwitchUnitScene::~SubmissionSwitchUnitScene(){}

std::shared_ptr<BattleSceneElement> SubmissionSwitchUnitScene::GetPlayerMoveScene()const{
	return std::shared_ptr<BattleSceneElement>(new SubmissionPlayerMoveScene(m_battleSceneData));
}
