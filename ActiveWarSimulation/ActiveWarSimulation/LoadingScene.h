#ifndef DEF_LOADINGSCENE_H
#define DEF_LOADINGSCENE_H

#include"GameScene.h"
#include<thread>
#include<atomic>

//ロード中に少しの画面変更をしたい時の繋ぎの場面
//スレッドを用いて実現する
class LoadingScene:public GameScene{
public:
	class LoadingSceneFactory:public SceneFactory{
	public:
		LoadingSceneFactory(const std::shared_ptr<GameScene::SceneFactory> &nextFactory);
		~LoadingSceneFactory();
		std::shared_ptr<GameScene> CreateIncompleteScene()const{
			return std::shared_ptr<GameScene>(new LoadingScene(m_nextFactory));
		}
	private:
		const std::shared_ptr<GameScene::SceneFactory> m_nextFactory;
	};

	virtual ~LoadingScene();
	void InitCompletely();
	void Activate();
	int Calculate();
	void Draw()const;

protected:
	LoadingScene(const std::shared_ptr<GameScene::SceneFactory> &nextFactory);
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;
	
private:
	FpsMeasuring m_timer;	//描画用のタイマー
	std::thread m_loadingThread;	//次の場面を読み込むスレッド
	//以下は全てm_loadingThreadで変更されるので、スレッド作成以降はmutexを用いてアクセスする(atomic_boolがこの役割を果たしてくれる)
	std::shared_ptr<GameScene> m_nextScene;	//読み込み先
	std::atomic_bool m_loadingEnd;	//読み込みが終わったかどうか（mutexの状態だけでは、スレッドが動き始める前にmutexでの状態判定が行われたら意図しない動作になってしまうため）
};

#endif // !DEF_LOADINGSCENE_H
