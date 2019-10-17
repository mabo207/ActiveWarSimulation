#ifndef DEF_SWITCHUNITINITORDERSCENE_H
#define DEF_SWITCHUNITINITORDERSCENE_H

#include"SwitchUnitScene.h"

//コンストラクタでユニットの行動順を初期化するSwitchUnitScene
//コミケ段階でのSwitchUnitSceneと同じ挙動をする

class SwitchUnitInitOrderScene:public SwitchUnitScene{
public:
	SwitchUnitInitOrderScene(std::shared_ptr<BattleSceneData> battleSceneData);
	virtual ~SwitchUnitInitOrderScene();
};

#endif // !DEF_SWITCHUNITINITORDERSCENE_H
