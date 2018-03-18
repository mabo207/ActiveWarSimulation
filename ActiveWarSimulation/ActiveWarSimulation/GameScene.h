#ifndef DEF_GAMESCENE_H
#define DEF_GAMESCENE_H

#include"ToolsLib.h"

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
	GameScene *m_pActivateClass;//���ۂɓ�������ʃN���X

	//�֐�
public:
	FadeInOutGameScene(GameScene *pActivateClass,int fadeFlag,int flame);
	~FadeInOutGameScene();
	int Calculate();
	void Draw()const;
};


#endif // !DEF_GAMESCENE_H

#pragma once
