#include"TutorialScene.h"
#include"TutorialBattleSceneData.h"

//--------------------TutorialScene--------------------
TutorialScene::TutorialScene(const std::string &stageName)
	:BattleScene(std::shared_ptr<BattleSceneData>(new TutorialBattleSceneData(stageName)))
{
}

TutorialScene::~TutorialScene(){}
