#include"SubmissionBattleScene.h"
#include"TutorialBattleSceneData.h"

#include"SwitchUnitInitOrderScene.h"
#include"SubmissionSwitchUnitScene.h"

#include"StageSelectScene.h"
#include"CreditScene.h"

//----------------SubmissionBattleScene::SubmissionBattleSceneFactory--------------------
SubmissionBattleScene::SubmissionBattleSceneFactory::SubmissionBattleSceneFactory(const std::string &stageDirName,const std::string &titleName,const StageLevel level)
	:SceneFactory()
	,m_stageDirName(stageDirName)
	,m_titleName(titleName)
	,m_level(level)
{}

SubmissionBattleScene::SubmissionBattleSceneFactory::~SubmissionBattleSceneFactory(){}

std::shared_ptr<GameScene> SubmissionBattleScene::SubmissionBattleSceneFactory::CreateIncompleteScene()const{
	return std::shared_ptr<GameScene>(new SubmissionBattleScene(m_stageDirName,m_titleName,m_level));
}

//----------------SubmissionBattleScene--------------------
SubmissionBattleScene::SubmissionBattleScene(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel)
	:BattleScene(std::shared_ptr<BattleSceneData>(new TutorialBattleSceneData(stageDirName,titleName,stageLevel)))
{}

SubmissionBattleScene::~SubmissionBattleScene(){}

std::shared_ptr<BattleSceneElement> SubmissionBattleScene::VGetSwitchUnitScene()const{
	//return std::shared_ptr<BattleSceneElement>(new SwitchUnitInitOrderScene(m_battleSceneData));
	return std::shared_ptr<BattleSceneElement>(new SubmissionSwitchUnitScene(m_battleSceneData));
}

std::shared_ptr<GameScene> SubmissionBattleScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//ゲームプレイが終わった時は、基本的にはステージセレクト画面へ
	if(!m_battleSceneData->m_gotoCredit){
		const auto stageSelectFactory=std::make_shared<StageSelectScene::StageSelectSceneFactory>();
		return CreateFadeOutInSceneCompletely(thisSharedPtr,stageSelectFactory,15,15);
	} else{
		//最終ステージクリア時のみ、クレジットへ
		const auto creditFactory=std::make_shared<CreditScene::CreditSceneFactory>();
		return CreateFadeOutInSceneCompletely(thisSharedPtr,creditFactory,15,15);
	}
}
