#ifndef DEF_SELECTLOGSCENE_H
#define DEF_SELECTLOGSCENE_H

#include"GameScene.h"
#include<string>
#include<vector>

class SelectLogScene:public GameScene{
public:
	class SelectLogSceneFactory:public SceneFactory{
	public:
		SelectLogSceneFactory(){}
		~SelectLogSceneFactory(){}
		std::shared_ptr<GameScene> CreateIncompleteScene()const{
			return std::shared_ptr<GameScene>(new SelectLogScene());
		}
	};
	
	SelectLogScene();
	~SelectLogScene();
	void InitCompletely();//メインスレッド以外で行うことができる初期化処理群
	void Activate();//初期化が終了し、このSceneの動作が始まる時に行う処理。BGMの再生処理など。
	int Calculate();
	void Draw()const;

private:
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;//場面遷移をする時に、どこに移動するかを決める関数。

	std::vector<std::string> m_fileNameList;
	std::vector<std::string>::const_iterator m_selectFileIt;
};

#endif // !DEF_SELECTLOGSCENE_H
