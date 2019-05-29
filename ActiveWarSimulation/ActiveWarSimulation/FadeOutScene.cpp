#include"FadeOutScene.h"
#include"DxLib.h"
#include"CommonConstParameter.h"

//----------------FadeOutScene-------------------
FadeOutScene::FadeOutScene(const std::shared_ptr<GameScene> &previousScene,const std::shared_ptr<GameScene::SceneFactory> &nextFactory,int maxFrame)
	:m_drawAlpha(0,255,maxFrame,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0)
	,m_previousScene(previousScene)
	,m_nextFactory(nextFactory)
	,m_afterAlphaEndFrame(0)
{}

FadeOutScene::~FadeOutScene(){}

int FadeOutScene::Calculate(){
	// �t�F�[�h�A�E�g����
	m_drawAlpha.Update();
	if(m_drawAlpha.GetEndFlag()){
		// �t�F�[�h�A�E�g���I��������m_afterAlphaEndFrame�����Z
		m_afterAlphaEndFrame++;
	}
	if(m_afterAlphaEndFrame>2){
		// �t�F�[�h�A�E�g���I�����Ă���2f�o�����玟�̏�ʂ�
		return 1;
	}
	return 0;
}

void FadeOutScene::Draw()const{
	//�w�i�F�̎擾
	unsigned int backColor;
	{
		int red,green,blue;
		GetBackgroundColor(&red,&green,&blue);
		backColor=GetColor(red,green,blue);
	}
	//��ʂ̕`��
	if(m_previousScene){
		//m_previousScene�����݂��Ă��邩�̔���͂��Ă���
		m_previousScene->Draw();
	}
	//�����x�̕\��(��ɔw�i�F�����`��킹��)
	if(m_drawAlpha.GetX()>0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,m_drawAlpha.GetX());
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,backColor,TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}
}

std::shared_ptr<GameScene> FadeOutScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//m_nextFactory��p���č쐬
	if(m_nextFactory){
		//m_nextFactory�����̂������ǂ���
		return m_nextFactory->CreateScene();
	}
	return std::shared_ptr<GameScene>();
}
