#ifndef DEF_DEMOSWITCHUNITSCENE_H
#define DEF_DEMOSWITCHUNITSCENE_H

#include"SwitchUnitScene.h"

//�f���p��SwitchUnitScene
class DemoSwitchUnitScene:public SwitchUnitScene{
	//�^�E�񋓑�

	//�萔

	//�ϐ�

	//�֐�
protected:
	
public:
	DemoSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~DemoSwitchUnitScene();
	
	int UpdateNextScene(int index);//SwitchUnitScene�ƈႢ�A�v���C���[�L������AI����ł���A���Q�[�����I������珉��������čŏ������蒼�����
};

#endif // !DEF_DEMOSWITCHUNITSCENE_H
