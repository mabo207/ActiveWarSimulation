#ifndef DEF_DEMOSWITCHUNITSCENE_H
#define DEF_DEMOSWITCHUNITSCENE_H

#include"SwitchUnitInitOrderScene.h"

//�f���p��SwitchUnitInitOrderScene
class DemoSwitchUnitScene:public SwitchUnitInitOrderScene{
	//�^�E�񋓑�

	//�萔

	//�ϐ�

	//�֐�
protected:

public:
	DemoSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~DemoSwitchUnitScene();

	int UpdateNextScene(int index);//SwitchUnitInitOrderScene�ƈႢ�A�v���C���[�L������AI����ł���A���Q�[�����I������珉��������čŏ������蒼�����
};

#endif // !DEF_DEMOSWITCHUNITSCENE_H
