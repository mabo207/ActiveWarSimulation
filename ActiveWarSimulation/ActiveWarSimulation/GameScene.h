#ifndef DEF_GAMESCENE_H
#define DEF_GAMESCENE_H

#include"ToolsLib.h"
#include<memory>

//場面を表すクラスが持つべき実装のインターフェース
//かつこれだけあれば他クラスから動かすことが出来る
class GameScene{
	//型
public:
	class SceneFactory{
		//GameSceneクラスを作るために必要なデータ群
	protected:
		SceneFactory(){}
		virtual ~SceneFactory(){}
	public:
		virtual std::shared_ptr<GameScene> CreateScene()const=0;//こいつをオーバーライド必須にして、各継承先クラスにFactoryを作らせる
	};
	//定数

	//変数
	//関数
protected:
	GameScene(){}
	virtual ~GameScene(){}
	virtual std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const=0;//場面遷移をする時に、どこに移動するかを決める関数。この値の設定方法は、クラスごとに実装方法を変えて良い。

public:
	virtual int Calculate()=0;
	virtual void Draw()const=0;
	//以下は、VGetNextScene()の呼び方を一定にするために用いる
	static std::shared_ptr<GameScene> GetNextScene(const std::shared_ptr<GameScene> &scene){
		if(scene){
			//自分自身を引数に入れてVNextSceneを呼び出す
			return scene->VGetNextScene(scene);
		}
		return std::shared_ptr<GameScene>();
	}
};

//フェードイン、フェードアウトをする機能をGameSceneに追加するクラス
//実際に動くクラスをpClassとして、pClassを動的確保してからFadeInOutGameScene(pClass,int,int)とする事で宣言できる。
class FadeInOutGameScene:public GameScene{
	//型
public:
	enum FADEFLAG{
		FADEIN=0x01,
		FADEOUT=0x02
	};
	class FadeInOutSceneFactory:public SceneFactory{
	public:
		FadeInOutSceneFactory(const std::shared_ptr<SceneFactory> &i_factory,int i_fadeFlag,int i_frame)
			:SceneFactory(),factory(i_factory),fadeFlag(i_fadeFlag),frame(i_frame){}
		~FadeInOutSceneFactory(){}
		std::shared_ptr<GameScene> CreateScene()const{
			return std::shared_ptr<GameScene>(new FadeInOutGameScene(factory,fadeFlag,frame));
		}

	private:
		const std::shared_ptr<SceneFactory> factory;
		const int fadeFlag;
		const int frame;
	};
	//定数

	//変数
protected:
	const int m_fadeFlag;//フェードイン、フェードアウトするかのフラグ管理をしている
	Easing m_drawAlpha;//描画透明度をxに管理
	int m_nowProcess;//現在の段階(0:通常進行 1:Calculate()を行わずにフェードアウト 2:フェードアウト完了待ち)
	int m_retIndex;//フェードアウト終了後Calculate()が返す数
	int m_timer;//フェードアウトが綺麗に行われるために用いる
	std::shared_ptr<GameScene> m_pActivateClass;//実際に動かす場面クラス

	//関数
protected:
	FadeInOutGameScene(const std::shared_ptr<GameScene::SceneFactory> &pActiveClassFactory,int fadeFlag,int frame);
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;//場面遷移をする時に、どこに移動するかを決める関数。この値の設定方法は、クラスごとに実装方法を変えて良い。

public:
	virtual ~FadeInOutGameScene();
	int Calculate();
	void Draw()const;
};

#endif // !DEF_GAMESCENE_H

