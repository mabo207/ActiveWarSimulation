#ifndef DEF_DEMOSCENE_H
#define DEF_DEMOSCENE_H

#include"BattleScene.h"

//�f�����s���N���X(�R���X�g���N�^��BattleScene.cpp�ɂ�SwitchUnitScene�������Ƃ����ς���Ηǂ�����)
class DemoScene:public BattleScene{
	//�^�E�񋓑�
public:
	class DemoSceneFactory:public MainSceneFactory{
	public:
		DemoSceneFactory();
		~DemoSceneFactory();
		std::shared_ptr<MainControledGameScene> CreateScene()const;

	protected:
		//�K�v�ȕϐ��͓��ɑ��݂��Ȃ�
	};

	//�萔
protected:

public:

	//�ϐ�
protected:
	int m_frame;//DemoScene��p�̃t���[�����J�E���^
	int m_font;//���ӏ����̃t�H���g

   //�֐�
protected:
	DemoScene();

public:
	~DemoScene();
	
	//���z�֐��̃I�[�o�[���C�h
	std::shared_ptr<BattleSceneElement> VGetSwitchUnitScene()const;//SwitchUnitScene��DemoSwitchUnitScene�̂ǂ������g����
	int Calculate();
	void Draw()const;

};

#endif // !DEF_DEMOSCENE_H
