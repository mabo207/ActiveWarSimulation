#include"SwitchUnitInitOrderScene.h"

//-----------------SwitchUnitInitOrderScene----------------
SwitchUnitInitOrderScene::SwitchUnitInitOrderScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:SwitchUnitScene(battleSceneData)
{
	//�I�[�_�[�̏��������s��
	ReturnProcess();
}

SwitchUnitInitOrderScene::~SwitchUnitInitOrderScene(){}
