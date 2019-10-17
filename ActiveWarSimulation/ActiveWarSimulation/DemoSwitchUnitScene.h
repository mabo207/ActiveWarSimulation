#ifndef DEF_DEMOSWITCHUNITSCENE_H
#define DEF_DEMOSWITCHUNITSCENE_H

#include"SwitchUnitInitOrderScene.h"

//デモ用のSwitchUnitInitOrderScene
class DemoSwitchUnitScene:public SwitchUnitInitOrderScene{
	//型・列挙体

	//定数

	//変数

	//関数
protected:

public:
	DemoSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~DemoSwitchUnitScene();

	int UpdateNextScene(int index);//SwitchUnitInitOrderSceneと違い、プレイヤーキャラもAI操作であり、かつゲームが終わったら初期化されて最初からやり直される
};

#endif // !DEF_DEMOSWITCHUNITSCENE_H
