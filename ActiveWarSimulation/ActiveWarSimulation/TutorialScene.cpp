#include"TutorialScene.h"
#include"TutorialBattleSceneData.h"
#include"ToolsLib.h"
#include"FileRead.h"

//--------------------TutorialScene::TutorialSceneFactory--------------------
TutorialScene::TutorialSceneFactory::TutorialSceneFactory(const std::string &stageDirName)
	:m_stageDirName(stageDirName)
{
	//stageInfo.txtの生データを読み込み可能な形式に変換
	const StringBuilder infoBuilder(FileStrRead(("Stage/"+m_stageDirName+"/stageInfo.txt").c_str()),',','(',')',true,true);
	for(const StringBuilder &sb:infoBuilder.GetVector()){
		if(sb.m_vec.size()>=2){
			if(sb.m_vec[0].GetString()=="title"){
				m_titleName=sb.m_vec[1].GetString();
			} else if(sb.m_vec[0].GetString()=="level"){
				m_stageLevel=std::atoi(sb.m_vec[1].GetString().c_str());
			}
		}
	}
}

TutorialScene::TutorialSceneFactory::TutorialSceneFactory(const std::string &stageDirName,const std::string &titleName,const int stageLevel)
	:m_stageDirName(stageDirName)
	,m_titleName(titleName)
	,m_stageLevel(stageLevel)
{}

TutorialScene::TutorialSceneFactory::~TutorialSceneFactory(){}

std::shared_ptr<MainControledGameScene> TutorialScene::TutorialSceneFactory::CreateScene()const{
	return std::shared_ptr<TutorialScene>(new TutorialScene(m_stageDirName,m_titleName,m_stageLevel));
}

//--------------------TutorialScene--------------------
TutorialScene::TutorialScene(const std::string &stageDirName,const std::string &titleName,const int stageLevel)
	:BattleScene(std::shared_ptr<BattleSceneData>(new TutorialBattleSceneData(stageDirName,titleName,stageLevel)))
{}

TutorialScene::~TutorialScene(){}
