#include"DemoScene.h"
#include"DemoSwitchUnitScene.h"

//--------------------DemoScene-------------------
DemoScene::DemoScene()
	:BattleScene("demo")
{
	//m_sceneDataの初期化、最初はデモ用の切り替えシーンで
	m_sceneData=std::shared_ptr<BattleSceneElement>(new DemoSwitchUnitScene(m_battleSceneData));
}

DemoScene::~DemoScene(){}

