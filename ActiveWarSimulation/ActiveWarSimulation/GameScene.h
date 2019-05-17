#ifndef DEF_GAMESCENE_H
#define DEF_GAMESCENE_H

#include"ToolsLib.h"
#include<memory>

//��ʂ�\���N���X�����ׂ������̃C���^�[�t�F�[�X
//�����ꂾ������Α��N���X���瓮�������Ƃ��o����
class GameScene{
	//�^
	//�萔

	//�ϐ�
	//�֐�
public:
	virtual int Calculate()=0;
	virtual void Draw()const=0;
	GameScene(){}
	virtual ~GameScene(){}
};

//main�֐��ŊǗ������ׂ��N���X�̃C���^�[�t�F�[�X
class MainControledGameScene:public GameScene{
	//�^
public:
	class MainSceneFactory{
		//MainControledGameScene�N���X����邽�߂ɕK�v�ȃf�[�^�Q
	protected:
		MainSceneFactory(){}
		virtual ~MainSceneFactory(){}
	public:
		virtual std::shared_ptr<MainControledGameScene> CreateScene()const=0;//�������I�[�o�[���C�h�K�{�ɂ��āA�e�p����N���X��Factory����点��
	};

	//�萔
	//�ϐ�

	//�֐�
public:
	virtual std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const=0;//��ʑJ�ڂ����鎞�ɁA�ǂ��Ɉړ����邩�����߂�֐��B���̒l�̐ݒ���@�́A�N���X���ƂɎ������@��ς��ėǂ��B
	MainControledGameScene():GameScene(){}
	virtual ~MainControledGameScene(){}
};

//�t�F�[�h�C���A�t�F�[�h�A�E�g������@�\��GameScene�ɒǉ�����N���X
//���ۂɓ����N���X��pClass�Ƃ��āApClass�𓮓I�m�ۂ��Ă���FadeInOutGameScene(pClass,int,int)�Ƃ��鎖�Ő錾�ł���B
class FadeInOutGameScene:public GameScene{
	//�^
public:
	enum FADEFLAG{
		FADEIN=0x01,
		FADEOUT=0x02
	};
	//�萔

	//�ϐ�
protected:
	const int m_fadeFlag;//�t�F�[�h�C���A�t�F�[�h�A�E�g���邩�̃t���O�Ǘ������Ă���
	Easing m_drawAlpha;//�`�擧���x��x�ɊǗ�
	int m_nowProcess;//���݂̒i�K(0:�ʏ�i�s 1:Calculate()���s�킸�Ƀt�F�[�h�A�E�g 2:�t�F�[�h�A�E�g�����҂�)
	int m_retIndex;//�t�F�[�h�A�E�g�I����Calculate()���Ԃ���
	int m_timer;//�t�F�[�h�A�E�g���Y��ɍs���邽�߂ɗp����
	std::shared_ptr<GameScene> m_pActivateClass;//���ۂɓ�������ʃN���X

	//�֐�
public:
	FadeInOutGameScene(std::shared_ptr<GameScene> pActivateClass,int fadeFlag,int frame);
	virtual ~FadeInOutGameScene();
	int Calculate();
	void Draw()const;
};

//FadeInOutGameScene��MainControledGameScene�ɑ΂��Ă��p�����悤�ɂ���
class MainControledFadeInOutGameScene:public FadeInOutGameScene,public MainControledGameScene{
	//�ϐ�
	//m_pActivateClass�͕K��MainControledGameScene�ł���

	//�֐�
public:
	MainControledFadeInOutGameScene(std::shared_ptr<MainControledGameScene> pActivateClass,int fadeFlag,int frame);
	~MainControledFadeInOutGameScene(){}
	int Calculate(){
		return FadeInOutGameScene::Calculate();
	}
	void Draw()const{
		FadeInOutGameScene::Draw();
	}
	std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const;//��ʑJ�ڂ����鎞�ɁA�ǂ��Ɉړ����邩�����߂�֐��B���̒l�̐ݒ���@�́A�N���X���ƂɎ������@��ς��ėǂ��B
};

#endif // !DEF_GAMESCENE_H

