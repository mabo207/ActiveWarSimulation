#include"SubmissionBattleScene.h"
#include"TutorialBattleSceneData.h"
#include"FilePath.h"

#include"SubmissionSwitchUnitScene.h"

#include"StageSelectScene.h"
#include"CreditScene.h"

//サブミッション一覧
#include"ArcherAttackDistance.h"
#include"MageAttackingOpponent.h"
#include"ArmerPosition.h"
#include"HealerPosition.h"
#include"ProtectFriend.h"
#include"IntensiveAttack.h"

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
{
	if(m_battleSceneData->m_playMode==BattleSceneData::PlayMode::e_tutorial){
		const std::shared_ptr<TutorialBattleSceneData> tutorialData=std::dynamic_pointer_cast<TutorialBattleSceneData>(m_battleSceneData);
		if(tutorialData){
			//キャスト後のnullptrチェックにクリアしたら、ExplainTutorialを入れる
			tutorialData->m_tutorialData.push_back(std::shared_ptr<TutorialBattleSceneData::TutorialBase>(new TutorialBattleSceneData::ExplainTutorial((FilePath::graphicDir+"tutorial/nonfree/"+"ArcherDistance.png").c_str())));
			//チュートリアルを設定する
			m_battleSceneData->m_scoreObserver->SetSubmissionRule(std::shared_ptr<SubmissionRuleBase>(new ArcherAttackDistance()));
		}
	}
}

SubmissionBattleScene::~SubmissionBattleScene(){}

std::shared_ptr<BattleSceneElement> SubmissionBattleScene::VGetSwitchUnitScene()const{
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
