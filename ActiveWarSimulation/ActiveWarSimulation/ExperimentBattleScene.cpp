#include"ExperimentBattleScene.h"
#include"TitleScene.h"

//----------------ExperimentBattleScene-------------------
ExperimentBattleScene::ExperimentBattleSceneFactory::ExperimentBattleSceneFactory(const std::string &stageDirName)
	:SceneFactory(),m_stageDirName(stageDirName){}

ExperimentBattleScene::ExperimentBattleSceneFactory::~ExperimentBattleSceneFactory(){}

std::shared_ptr<GameScene> ExperimentBattleScene::ExperimentBattleSceneFactory::CreateIncompleteScene()const{
	return std::shared_ptr<GameScene>(new ExperimentBattleScene(m_stageDirName));
}

ExperimentBattleScene::ExperimentBattleScene(const std::string &stageDirName)
	:BattleScene(stageDirName,"",StageLevel::e_normal){}

ExperimentBattleScene::~ExperimentBattleScene(){}

std::shared_ptr<GameScene> ExperimentBattleScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//ゲームプレイが終わった時は、ステージセレクト画面へ
	const auto titleFactory=std::make_shared<TitleScene::TitleSceneFactory>();
	return CreateFadeOutInSceneCompletely(thisSharedPtr,titleFactory,15,15);
}
