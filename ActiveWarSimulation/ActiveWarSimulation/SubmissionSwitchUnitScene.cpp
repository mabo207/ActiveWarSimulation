#include"SubmissionSwitchUnitScene.h"
#include"SelectSubmissionScene.h"

//-------------SubmissionSwitchUnitScene--------------
SubmissionSwitchUnitScene::SubmissionSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:SwitchUnitScene(battleSceneData)
{
	//m_nextScene��SelectSubmissionScene��ǉ�
	m_nextScene=std::shared_ptr<BattleSceneElement>(new SelectSubmissionScene(battleSceneData));
}

SubmissionSwitchUnitScene::~SubmissionSwitchUnitScene(){}
