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
		std::shared_ptr<GameScene> CreateScene()const{
			return std::shared_ptr<GameScene>(new FadeOutScene(previousScene,nextFactory,maxFrame));
		}

	private:
		const std::shared_ptr<GameScene> previousScene;
		const std::shared_ptr<GameScene::SceneFactory> nextFactory;
		const int maxFrame;
	};

	virtual ~FadeOutScene();
	int Calculate();
	void Draw()const;
	std::shared_ptr<GameScene> VGetNextScene()const;

protected:
	FadeOutScene(const std::shared_ptr<GameScene> &previousScene,const std::shared_ptr<GameScene::SceneFactory> &nextFactory,int maxFrame);

private:
	Easing m_drawAlpha;		// 描画透明度をxに管理
	const std::shared_ptr<GameScene> m_previousScene;	// 直前場面。描画のために使う。
	const std::shared_ptr<GameScene::SceneFactory> m_nextFactory;	// 次の場面、描画の情報は得たいので実体を作成しておく
};

#endif // !DEF_FADEINSCENE_H