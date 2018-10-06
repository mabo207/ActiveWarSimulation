#include"DemoScene.h"
#include"DemoSwitchUnitScene.h"

//--------------------DemoScene-------------------
DemoScene::DemoScene()
	:BattleScene("demo")
{
	//m_sceneData�̏���������蒼���ABattleScene�̃R���X�g���N�^���ł�DemoScene�̊֐��͌Ăяo���Ȃ�
	m_sceneData=VGetSwitchUnitScene();
}

DemoScene::~DemoScene(){}

std::shared_ptr<BattleSceneElement> DemoScene::VGetSwitchUnitScene()const{
	return std::shared_ptr<BattleSceneElement>(new DemoSwitchUnitScene(m_battleSceneData));
}

