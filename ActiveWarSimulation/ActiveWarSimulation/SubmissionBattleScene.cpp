#include"SubmissionBattleScene.h"
#include"TutorialBattleSceneData.h"
#include"FilePath.h"
#include"StringBuilder.h"
#include"FileRead.h"

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
			//キャスト後のnullptrチェックにクリアしたら、サブミッション作成作業
			std::string graphName;
			std::shared_ptr<SubmissionRuleBase> rule;
			try{
				const StringBuilder sb(FileStrRead((FilePath::stageDir+m_battleSceneData->m_stageDirName+"/submission.txt").c_str()),',','(',')');
				for(const StringBuilder &ssb:sb.m_vec){
					if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="kind"){
						//ssb.m_vec[1].GetString()がタグになる
						const std::string tag=ssb.m_vec[1].GetString();
						if(tag=="ArcherDistance"){
							graphName="ArcherDistance.png";
							rule=std::shared_ptr<SubmissionRuleBase>(new ArcherAttackDistance());
						} else if(tag=="ArmerPosition"){
							graphName="ArmerPosition.png";
							rule=std::shared_ptr<SubmissionRuleBase>(new ArmerPosition());
						} else if(tag=="HealerPosition"){
							graphName="HealerPosition.png";
							rule=std::shared_ptr<SubmissionRuleBase>(new HealerPosition());
						} else if(tag=="IntensiveAttack"){
							graphName="IntensiveAttack.png";
							rule=std::shared_ptr<SubmissionRuleBase>(new IntensiveAttack());
						} else if(tag=="MageAttackingOpponent"){
							graphName="MageAttackingOpponent.png";
							rule=std::shared_ptr<SubmissionRuleBase>(new MageAttackingOpponent());
						} else if(tag=="ProtectFriend"){
							graphName="ProtectFriend.png";
							rule=std::shared_ptr<SubmissionRuleBase>(new ProtectFriend());
						}
					}
				}
			}catch(const FileOpenFailedException &){
				int a=0;
				int b=a;
			}
			//サブミッションが作成できた場合
			if(rule){
				//ExplainTutorialを入れる
				tutorialData->m_tutorialData.push_back(std::shared_ptr<TutorialBattleSceneData::TutorialBase>(new TutorialBattleSceneData::ExplainTutorial((FilePath::graphicDir+"tutorial/nonfree/"+graphName).c_str())));
				//サブミッションを設定する
				m_battleSceneData->m_scoreObserver->SetSubmissionRule(rule);
			}
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
