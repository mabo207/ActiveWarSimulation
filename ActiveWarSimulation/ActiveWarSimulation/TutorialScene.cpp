#include"TutorialScene.h"
#include"TutorialBattleSceneData.h"
#include"ToolsLib.h"
#include"FileRead.h"
#include"StageInfoReader.h"

//--------------------TutorialScene::TutorialSceneFactory--------------------
TutorialScene::TutorialSceneFactory::TutorialSceneFactory(const std::string &stageDirName)
	:m_stageDirName(stageDirName)
{
	const StageInfoReader reader(stageDirName);
	m_titleName=reader.GetTitleName();
}

TutorialScene::TutorialSceneFactory::TutorialSceneFactory(const std::string &stageDirName,const std::string &titleName)
	:m_stageDirName(stageDirName)
	,m_titleName(titleName)
{}

TutorialScene::TutorialSceneFactory::~TutorialSceneFactory(){}

std::shared_ptr<GameScene> TutorialScene::TutorialSceneFactory::CreateScene()const{
	return std::shared_ptr<TutorialScene>(new TutorialScene(m_stageDirName,m_titleName,StageLevel::e_easy));
}

//--------------------TutorialScene--------------------
TutorialScene::TutorialScene(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel)
	:BattleScene(std::shared_ptr<BattleSceneData>(new TutorialBattleSceneData(stageDirName,titleName,stageLevel)))
{}

TutorialScene::~TutorialScene(){}
