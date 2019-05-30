#ifndef DEF_FADEINSCENE_H
#define DEF_FADEINSCENE_H

//シーンとシーンの合間のフェードインをする隙間場面
//次のシーンはFadeInSceneの作成と同時に作成される
#include"GameScene.h"
#include"ToolsLib.h"

// フェードインするシーン
class FadeInScene: public GameScene
{
public:
	class FadeInSceneFactory:public SceneFactory{
	public:
		FadeInSceneFactory(const std::shared_ptr<GameScene::SceneFactory> &i_nextFactory,const int i_maxFrame)
			:nextFactory(i_nextFactory),maxFrame(i_maxFrame){}
		~FadeInSceneFactory(){}
		std::shared_ptr<GameScene> CreateScene()const{
			return std::shared_ptr<GameScene>(new FadeInScene(nextFactory,maxFrame));
		}

	private:
		const std::shared_ptr<GameScene::SceneFactory> nextFactory;
		const int maxFrame;
	};

	virtual ~FadeInScene();
	int Calculate();
	void Draw()const;

protected:
	FadeInScene(const std::shared_ptr<GameScene::SceneFactory> &nextFactory,int maxFrame);
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;

private:
	Easing m_drawAlpha;		// 描画透明度をxに管理
	const std::shared_ptr<GameScene> m_nextScene;	// 次の場面、描画の情報は得たいので実体を作成しておく
	int m_afterAlphaEndFrame;	// m_drawAlphaの変動が終了してから何フレーム経ったか。2f経った後でないと画面が完全に暗転しない。
};

#endif // !DEF_FADEINSCENE_H