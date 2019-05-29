#include"FadeInScene.h"
#include"DxLib.h"
#include"CommonConstParameter.h"

//----------------FadeInScene-------------------
FadeInScene::FadeInScene(const std::shared_ptr<GameScene::SceneFactory> &nextFactory,int maxFrame)
	:m_nextScene(nextFactory?nextFactory->CreateScene():std::shared_ptr<GameScene>())
	,m_drawAlpha(255,0,maxFrame,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0)
{}

FadeInScene::~FadeInScene(){}

int FadeInScene::Calculate(){
	// �t�F�[�h�C������
	m_drawAlpha.Update();
	if(m_drawAlpha.GetEndFlag()){
		// �t�F�[�h�C�����I�������玟�̏�ʂ�
		return 1;
	}
	return 0;
}

void FadeInScene::Draw()const{
	//�w�i�F�̎擾
	unsigned int backColor;
	{
		int red,green,blue;
		GetBackgroundColor(&red,&green,&blue);
		backColor=GetColor(red,green,blue);
	}
	//��ʂ̕`��
	if(m_nextScene){
		//m_nextScene�����݂��Ă��邩�̔���͂��Ă���
		m_nextScene->Draw();
	}
	//�����x�̕\��(��ɔw�i�F�����`��킹��)
	if(m_drawAlpha.GetX()>0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,m_drawAlpha.GetX());
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,backColor,TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}
}

std::shared_ptr<GameScene> FadeInScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//���̂܂ܕԂ�
	return m_nextScene;
}
