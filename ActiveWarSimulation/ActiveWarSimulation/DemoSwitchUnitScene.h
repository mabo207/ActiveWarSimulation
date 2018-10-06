#ifndef DEF_DEMOSWITCHUNITSCENE_H
#define DEF_DEMOSWITCHUNITSCENE_H

#include"SwitchUnitScene.h"

//デモ用のSwitchUnitScene
class DemoSwitchUnitScene:public SwitchUnitScene{
	//型・列挙体

	//定数

	//変数

	//関数
protected:
	//仮想関数のオーバーライド
	std::shared_ptr<BattleSceneElement> GetPlayerOperateClass()const;//プレイヤーキャラを操作するクラスをAI操作クラスとして取得する。

public:
	DemoSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~DemoSwitchUnitScene();
	
	
};

#endif // !DEF_DEMOSWITCHUNITSCENE_H
