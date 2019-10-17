#include"SwitchUnitInitOrderScene.h"

//-----------------SwitchUnitInitOrderScene----------------
SwitchUnitInitOrderScene::SwitchUnitInitOrderScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:SwitchUnitScene(battleSceneData)
{
	//オーダーの初期化を行う
	ReturnProcess();
}

SwitchUnitInitOrderScene::~SwitchUnitInitOrderScene(){}
