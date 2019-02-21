#include"TutorialScene.h"
#include"TutorialBattleSceneData.h"

//--------------------TutorialScene--------------------
TutorialScene::TutorialScene()
	:BattleScene(std::shared_ptr<BattleSceneData>(new TutorialBattleSceneData()))
{
}

TutorialScene::~TutorialScene(){}
