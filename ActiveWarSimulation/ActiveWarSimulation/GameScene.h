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
		virtual std::shared_ptr<GameScene> CreateIncompleteScene()const=0;//こいつをオーバーライド必須にして、各継承先クラスにFactoryを作らせる
		std::shared_ptr<GameScene> CreateCompleteScene()const{
			//InitCompletely(),Activate()をした状態のものを返す
			const std::shared_ptr<GameScene> scene=CreateIncompleteScene();
			scene->InitCompletely();
			scene->Activate();
			return scene;
		}
	};
	//定数

	//変数
	//関数
protected:
	GameScene(){}
	virtual ~GameScene(){}
	virtual std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const=0;//場面遷移をする時に、どこに移動するかを決める関数。この値の設定方法は、クラスごとに実装方法を変えて良い。

	static std::shared_ptr<GameScene> CreateFadeOutInSceneCompletely(const std::shared_ptr<GameScene> &thisSharedPtr,const std::shared_ptr<GameScene::SceneFactory> &nextFactory,const int fadeOutFrame,const int fadeInFrame);// フェードアウトしてからフェードインするクラスを作成する便利関数、completeである

public:
	virtual void InitCompletely()=0;//メインスレッド以外で行うことができる初期化処理群
	virtual void Activate()=0;//初期化が終了し、このSceneの動作が始まる時に行う処理。BGMの再生処理など。
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

#endif // !DEF_GAMESCENE_H

