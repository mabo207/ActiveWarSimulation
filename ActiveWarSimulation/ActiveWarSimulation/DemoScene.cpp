#include"DemoScene.h"
#include"DemoSwitchUnitScene.h"

//--------------------DemoScene-------------------
DemoScene::DemoScene()
	:BattleScene("01_tutorial")
{
	//m_sceneData�̏������A�ŏ��̓f���p�̐؂�ւ��V�[����
	m_sceneData=std::shared_ptr<BattleSceneElement>(new DemoSwitchUnitScene(m_battleSceneData));
}

DemoScene::~DemoScene(){}

