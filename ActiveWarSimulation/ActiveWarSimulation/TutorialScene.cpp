#include"TutorialScene.h"
#include"TutorialBattleSceneData.h"

//--------------------TutorialScene::TutorialSceneFactory--------------------
TutorialScene::TutorialSceneFactory::TutorialSceneFactory(const std::string &stageDirName)
	:m_stageDirName(stageDirName)
{}

TutorialScene::TutorialSceneFactory::~TutorialSceneFactory(){}

std::shared_ptr<MainControledGameScene> TutorialScene::TutorialSceneFactory::CreateScene()const{
	return std::shared_ptr<TutorialScene>(new TutorialScene(m_stageDirName));
}

//--------------------TutorialScene--------------------
TutorialScene::TutorialScene(const std::string &stageName)
	:BattleScene(std::shared_ptr<BattleSceneData>(new TutorialBattleSceneData(stageName)))
{}

TutorialScene::~TutorialScene(){}
