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
	//���z�֐��̃I�[�o�[���C�h
	std::shared_ptr<BattleSceneElement> GetPlayerOperateClass()const;//�v���C���[�L�����𑀍삷��N���X��AI����N���X�Ƃ��Ď擾����B

public:
	DemoSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~DemoSwitchUnitScene();
	
	
};

#endif // !DEF_DEMOSWITCHUNITSCENE_H
