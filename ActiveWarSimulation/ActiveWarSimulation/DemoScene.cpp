#include"DemoScene.h"
#include"DemoSwitchUnitScene.h"

//--------------------DemoScene-------------------
DemoScene::DemoScene()
	:BattleScene("demo")
{
	//m_sceneData�̏������A�ŏ��̓f���p�̐؂�ւ��V�[����
	m_sceneData=std::shared_ptr<BattleSceneElement>(new DemoSwitchUnitScene(m_battleSceneData));
}

DemoScene::~DemoScene(){}

