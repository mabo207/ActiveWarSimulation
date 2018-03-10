#include<iostream>
#include"DxLib.h"
#include"GameScene.h"

//---------------------FadeInOutGameScene---------------------
FadeInOutGameScene::FadeInOutGameScene(GameScene *pActivateClass,int fadeFlag,int flame)
	:GameScene(),m_fadeFlag(fadeFlag),m_nowProcess(0),m_retIndex(0),m_pActivateClass(pActivateClass),m_timer(0)
{
	//�t�F�[�h�C���̕`�擧���x�̐ݒ������߂�
	if((m_fadeFlag & FADEIN)!=0){
		//�t�F�[�h�C������ꍇ
		m_drawAlpha=Easing(255,flame,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
		m_drawAlpha.SetTarget(0,true);
	} else{
		m_drawAlpha=Easing(0,flame,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
	}
}

FadeInOutGameScene::~FadeInOutGameScene(){
	//m_pActivateClass�͓��I�m�ۂ���Ă���͂��Ȃ̂ŉ������
	if(m_pActivateClass!=NULL){
		delete m_pActivateClass;
		m_pActivateClass=NULL;
	}
}

int FadeInOutGameScene::Calculate(){
	//�����x�̍X�V
	m_drawAlpha.Update();
	//�t�F�[�h�A�E�g�I�����̒E�o
	if(m_nowProcess==1 && m_drawAlpha.GetEndFlag()){
		m_nowProcess=2;
	} else if(m_nowProcess==2){
		m_timer++;
		if(m_timer>2){
			return m_retIndex;
		}
	}
	//��ʃN���X��Calculate()
	int index;
	if(m_nowProcess==0){
		//m_nowProcess==0�̎��̂�Calculate()����
		index=m_pActivateClass->Calculate();
	} else{
		index=0;
	}
	//�����x�ݒ���̍X�V(�t�F�[�h�A�E�g�̌��o)
	if(index!=0){
		//�O��ʂɖ߂�w�����o����
		m_retIndex=index;
		if((m_fadeFlag & FADEOUT)!=0){
			//�t�F�[�h�A�E�g���s���ݒ�����Ă���ꍇ
			m_nowProcess=1;//����ȏ��ʃN���X��Calculate()���s��Ȃ�
			m_drawAlpha=Easing(m_drawAlpha.GetX(),m_drawAlpha.GetMaxFlame()
				,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
			m_drawAlpha.SetTarget(255,true);
		} else{
			//�t�F�[�h�A�E�g���s��Ȃ��ݒ�����Ă���ꍇ�͂��̂܂�return
			return index;
		}
	}
	return 0;
}

void FadeInOutGameScene::Draw(){
	//�w�i�F�̎擾
	unsigned int backColor;
	{
		int red,green,blue;
		GetBackgroundColor(&red,&green,&blue);
		backColor=GetColor(red,green,blue);
	}
	//��ʂ̕`��
	m_pActivateClass->Draw();
	//�����x�̕\��(��ɔw�i�F�����`��킹��)
	if(m_drawAlpha.GetX()>0){
		int dx,dy;
		GetWindowSize(&dx,&dy);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,m_drawAlpha.GetX());
		DrawBox(0,0,dx,dy,backColor,TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}
}
