#ifndef DEF_SWITCHUNITINITORDERSCENE_H
#define DEF_SWITCHUNITINITORDERSCENE_H

#include"SwitchUnitScene.h"

//�R���X�g���N�^�Ń��j�b�g�̍s����������������SwitchUnitScene
//�R�~�P�i�K�ł�SwitchUnitScene�Ɠ�������������

class SwitchUnitInitOrderScene:public SwitchUnitScene{
public:
	SwitchUnitInitOrderScene(std::shared_ptr<BattleSceneData> battleSceneData);
	virtual ~SwitchUnitInitOrderScene();
};

#endif // !DEF_SWITCHUNITINITORDERSCENE_H
