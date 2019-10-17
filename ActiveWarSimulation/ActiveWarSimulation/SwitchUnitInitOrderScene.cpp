#include"SwitchUnitInitOrderScene.h"

//-----------------SwitchUnitInitOrderScene----------------
SwitchUnitInitOrderScene::SwitchUnitInitOrderScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:SwitchUnitScene(battleSceneData)
{
	//進行データを更新
	ReturnProcess();
}

SwitchUnitInitOrderScene::~SwitchUnitInitOrderScene(){}
