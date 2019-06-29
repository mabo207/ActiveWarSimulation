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
	//並列的な処理を行わない
}

int LoadingScene::Calculate(){
	//タイマー更新
	m_timer.Update();
	//ロード作業が終了したかの判定
	if(m_loadingEnd.load(std::memory_order_acquire) && GetASyncLoadNum()==0){
		//終了してれば次の場面へ
		SetUseASyncLoadFlag(FALSE);//画像等の非同期読み込み設定を解除
		m_loadingThread.join();	//スレッドの管理を手放してデストラクタを呼べるようにする
		return 1;
	}
	return 0;
}

void LoadingScene::Draw()const{
	//とりあえず円を回す
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
	//コンストラクタが作成されてからの時間を測る
	m_timer.RecordTime();
	//画像等を非同期読み込みするように設定する
	SetUseASyncLoadFlag(TRUE);
	//並列処理をするまでの初期化をする
	m_nextScene=nextFactory->CreateIncompleteScene();
	//スレッドを作成し、読み込みをさせる
	std::shared_ptr<GameScene> &nextScene=m_nextScene;
	std::atomic_bool &loadingEnd=m_loadingEnd;
	m_loadingThread=std::thread([&nextScene,&loadingEnd]{
		//実はこれじゃだめ、LoadGraph()とかはスレッドセーフでないのでCreateScene()でグラフィックの読み込みとかが正常に行われない……
		nextScene->InitCompletely();
		loadingEnd.store(true,std::memory_order_release);
	});
}

std::shared_ptr<GameScene> LoadingScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//そのまま返す
	return m_nextScene;
}
