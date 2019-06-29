#define _USE_MATH_DEFINES
#include"LoadingScene.h"
#include"DxLib.h"
#include<cmath>

//----------------LoadingScene::LoadingSceneFactory--------------
LoadingScene::LoadingSceneFactory::LoadingSceneFactory(const std::shared_ptr<GameScene::SceneFactory> &nextFactory)
	:SceneFactory(),m_nextFactory(nextFactory)
{}

LoadingScene::LoadingSceneFactory::~LoadingSceneFactory(){}

LoadingScene::~LoadingScene(){}

void LoadingScene::InitCompletely(){
	//����I�ȏ������s��Ȃ�
}

int LoadingScene::Calculate(){
	//�^�C�}�[�X�V
	m_timer.Update();
	//���[�h��Ƃ��I���������̔���
	if(m_loadingEnd.load(std::memory_order_acquire) && GetASyncLoadNum()==0){
		//�I�����Ă�Ύ��̏�ʂ�
		SetUseASyncLoadFlag(FALSE);//�摜���̔񓯊��ǂݍ��ݐݒ������
		m_loadingThread.join();	//�X���b�h�̊Ǘ���������ăf�X�g���N�^���Ăׂ�悤�ɂ���
		return 1;
	}
	return 0;
}

void LoadingScene::Draw()const{
	//�Ƃ肠�����~����
	const double theta=m_timer.GetProcessedTime()*2*M_PI*3;
	const int r=20,x=1800+(int)(r*std::cos(theta)),y=1000+(int)(r*std::sin(theta));
	DrawCircle(x,y,4,GetColor(255,255,255),TRUE);
}

LoadingScene::LoadingScene(const std::shared_ptr<GameScene::SceneFactory> &nextFactory)
	:GameScene()
	,m_timer()
	,m_nextScene()
	,m_loadingEnd(false)
{
	//�R���X�g���N�^���쐬����Ă���̎��Ԃ𑪂�
	m_timer.RecordTime();
	//�摜����񓯊��ǂݍ��݂���悤�ɐݒ肷��
	SetUseASyncLoadFlag(TRUE);
	//���񏈗�������܂ł̏�����������
	m_nextScene=nextFactory->CreateIncompleteScene();
	//�X���b�h���쐬���A�ǂݍ��݂�������
	std::shared_ptr<GameScene> &nextScene=m_nextScene;
	std::atomic_bool &loadingEnd=m_loadingEnd;
	m_loadingThread=std::thread([&nextScene,&loadingEnd]{
		//���͂��ꂶ�Ⴞ�߁ALoadGraph()�Ƃ��̓X���b�h�Z�[�t�łȂ��̂�CreateScene()�ŃO���t�B�b�N�̓ǂݍ��݂Ƃ�������ɍs���Ȃ��c�c
		nextScene->InitCompletely();
		loadingEnd.store(true,std::memory_order_release);
	});
}

std::shared_ptr<GameScene> LoadingScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//���̂܂ܕԂ�
	return m_nextScene;
}