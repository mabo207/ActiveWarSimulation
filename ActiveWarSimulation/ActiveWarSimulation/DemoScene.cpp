#include"DemoScene.h"
#include"DemoSwitchUnitScene.h"

//--------------------DemoScene-------------------
DemoScene::DemoScene()
	:BattleScene("demo")
{
	//m_sceneDataの初期化をやり直す、BattleSceneのコンストラクタ内ではDemoSceneの関数は呼び出せない
	m_sceneData=VGetSwitchUnitScene();
}

DemoScene::~DemoScene(){}

std::shared_ptr<BattleSceneElement> DemoScene::VGetSwitchUnitScene()const{
	return std::shared_ptr<BattleSceneElement>(new DemoSwitchUnitScene(m_battleSceneData));
}

