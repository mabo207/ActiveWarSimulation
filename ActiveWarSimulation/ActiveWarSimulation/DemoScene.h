#ifndef DEF_DEMOSCENE_H
#define DEF_DEMOSCENE_H

#include"BattleScene.h"

//�f�����s���N���X(�R���X�g���N�^��BattleScene.cpp�ɂ�SwitchUnitScene�������Ƃ����ς���Ηǂ�����)
class DemoScene:public BattleScene{
	//�^�E�񋓑�
public:

	//�萔
protected:

public:

	//�ϐ�
protected:

   //�֐�
protected:
	int m_flame;//DemoScene��p�̃t���[�����J�E���^
	int m_font;//���ӏ����̃t�H���g

public:
	DemoScene();
	~DemoScene();
	
	//���z�֐��̃I�[�o�[���C�h
	std::shared_ptr<BattleSceneElement> VGetSwitchUnitScene()const;//SwitchUnitScene��DemoSwitchUnitScene�̂ǂ������g����
	int Calculate();
	void Draw()const;

};

#endif // !DEF_DEMOSCENE_H
