#ifndef DEF_FADEOUTSCENE_H
#define DEF_FADEOUTSCENE_H

//シーンとシーンの合間のフェードアウトをする隙間場面
//次のシーンはVGetNextScene()が呼ばれた時に作成される
#include"GameScene.h"
#include"ToolsLib.h"

// フェードアウトするシーン
class FadeOutScene: public GameScene
{
public:
	class FadeOutSceneFactory:public SceneFactory{
	public:
		FadeOutSceneFactory(const std::shared_ptr<GameScene> &i_previousScene,const std::shared_ptr<GameScene::SceneFactory> &i_nextFactory,const int i_maxFrame)
			:previousScene(i_previousScene),nextFactory(i_nextFactory),maxFrame(i_maxFrame){}
		~FadeOutSceneFactory(){}
		std::shared_ptr<GameScene> CreateIncompleteScene()const{
			return std::shared_ptr<GameScene>(new FadeOutScene(previousScene,nextFactory,maxFrame));
		}

	private:
		const std::shared_ptr<GameScene> previousScene;
		const std::shared_ptr<GameScene::SceneFactory> nextFactory;
		const int maxFrame;
	};

	virtual ~FadeOutScene();
	void InitCompletely();
	void Activate();
	int Calculate();
	void Draw()const;

protected:
	FadeOutScene(const std::shared_ptr<GameScene> &previousScene,const std::shared_ptr<GameScene::SceneFactory> &nextFactory,int maxFrame);
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;

private:
	Easing m_drawAlpha;		// 描画透明度をxに管理
	const std::shared_ptr<GameScene> m_previousScene;	// 直前場面。描画のために使う。
	const std::shared_ptr<GameScene::SceneFactory> m_nextFactory;	// 次の場面、描画の情報は得たいので実体を作成しておく
	int m_afterAlphaEndFrame;	// m_drawAlphaの変動が終了してから何フレーム経ったか。2f経った後でないと画面が完全に暗転しない。
};

#endif // !DEF_FADEINSCENE_H