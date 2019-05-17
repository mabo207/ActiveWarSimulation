#include<iostream>
#include"DxLib.h"
#include"GameScene.h"
#include"CommonConstParameter.h"

//---------------------GameScene---------------------


//---------------------FadeInOutGameScene---------------------
FadeInOutGameScene::FadeInOutGameScene(std::shared_ptr<GameScene> pActivateClass,int fadeFlag,int frame)
	:GameScene(),m_fadeFlag(fadeFlag),m_nowProcess(0),m_retIndex(0),m_pActivateClass(pActivateClass),m_timer(0)
{
	//�t�F�[�h�C���̕`�擧���x�̐ݒ������߂�
	if((m_fadeFlag & FADEIN)!=0){
		//�t�F�[�h�C������ꍇ
		m_drawAlpha=Easing(255,0,frame,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
	} else{
		m_drawAlpha=Easing(0,frame,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
	}
}

FadeInOutGameScene::~FadeInOutGameScene(){
	
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
			m_drawAlpha=Easing(m_drawAlpha.GetX(),255,m_drawAlpha.GetMaxFrame()
				,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
		} else{
			//�t�F�[�h�A�E�g���s��Ȃ��ݒ�����Ă���ꍇ�͂��̂܂�return
			return index;
		}
	}
	return 0;
}

void FadeInOutGameScene::Draw()const{
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
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,m_drawAlpha.GetX());
		//DrawBox(0,0,dx,dy,backColor,TRUE);
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,backColor,TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}
}

//--------------------MainControledFadeInOutGameScene--------------------
MainControledFadeInOutGameScene::MainControledFadeInOutGameScene(std::shared_ptr<MainControledGameScene> pActivateClass,int fadeFlag,int frame)
	:FadeInOutGameScene(pActivateClass,fadeFlag,frame),MainControledGameScene()
{}

std::shared_ptr<MainControledGameScene> MainControledFadeInOutGameScene::VGetNextMainControledScene()const{
	//m_pActivateClass���S��MainControledGameScene�ł��鎖��p����
	const MainControledGameScene *pScene=dynamic_cast<const MainControledGameScene *>(m_pActivateClass.get());
	if(pScene!=nullptr){
		return pScene->VGetNextMainControledScene();
	}
	return std::shared_ptr<MainControledGameScene>();
}
