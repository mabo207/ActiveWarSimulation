#include"DemoSwitchUnitScene.h"
#include"ComputerMoveScene.h"

//------------DemoSwitchUnitScene------------
DemoSwitchUnitScene::DemoSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:SwitchUnitScene(battleSceneData){}

DemoSwitchUnitScene::~DemoSwitchUnitScene(){}

std::shared_ptr<BattleSceneElement> DemoSwitchUnitScene::GetPlayerOperateClass()const{
	return std::shared_ptr<BattleSceneElement>(new ComputerMoveScene(m_battleSceneData));
}
