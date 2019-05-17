#ifndef DEF_GAMESCENE_H
#define DEF_GAMESCENE_H

#include"ToolsLib.h"
#include<memory>

//場面を表すクラスが持つべき実装のインターフェース
//かつこれだけあれば他クラスから動かすことが出来る
class GameScene{
	//型
	//定数

	//変数
	//関数
public:
	virtual int Calculate()=0;
	virtual void Draw()const=0;
	GameScene(){}
	virtual ~GameScene(){}
};

//main関数で管理されるべきクラスのインターフェース
class MainControledGameScene:public GameScene{
	//型
public:
	class MainSceneFactory{
		//MainControledGameSceneクラスを作るために必要なデータ群
	protected:
		MainSceneFactory(){}
		virtual ~MainSceneFactory(){}
	public:
		virtual std::shared_ptr<MainControledGameScene> CreateScene()const=0;//こいつをオーバーライド必須にして、各継承先クラスにFactoryを作らせる
	};

	//定数
	//変数

	//関数
public:
	virtual std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const=0;//場面遷移をする時に、どこに移動するかを決める関数。この値の設定方法は、クラスごとに実装方法を変えて良い。
	MainControledGameScene():GameScene(){}
	virtual ~MainControledGameScene(){}
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
public:
	FadeInOutGameScene(std::shared_ptr<GameScene> pActivateClass,int fadeFlag,int frame);
	virtual ~FadeInOutGameScene();
	int Calculate();
	void Draw()const;
};

//FadeInOutGameSceneをMainControledGameSceneに対しても用いれるようにした
class MainControledFadeInOutGameScene:public FadeInOutGameScene,public MainControledGameScene{
	//変数
	//m_pActivateClassは必ずMainControledGameSceneである

	//関数
public:
	MainControledFadeInOutGameScene(std::shared_ptr<MainControledGameScene> pActivateClass,int fadeFlag,int frame);
	~MainControledFadeInOutGameScene(){}
	int Calculate(){
		return FadeInOutGameScene::Calculate();
	}
	void Draw()const{
		FadeInOutGameScene::Draw();
	}
	std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const;//場面遷移をする時に、どこに移動するかを決める関数。この値の設定方法は、クラスごとに実装方法を変えて良い。
};

#endif // !DEF_GAMESCENE_H

