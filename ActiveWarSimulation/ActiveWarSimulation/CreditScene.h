#ifndef DEF_CREDITSCENE_H
#define DEF_CREDITSCENE_H

#include"GameScene.h"
#include<string>
#include<vector>
#include"BGM.h"

class CreditScene:public GameScene{
public:
	class CreditSceneFactory:public SceneFactory{
	public:
		CreditSceneFactory()=default;
		~CreditSceneFactory()=default;
		std::shared_ptr<GameScene> CreateIncompleteScene()const;
	};

	~CreditScene();
	void InitCompletely();//メインスレッド以外で行うことができる初期化処理群
	void Activate();//初期化が終了し、このSceneの動作が始まる時に行う処理。BGMの再生処理など。
	int Calculate();
	void Draw()const;

private:
	struct CreditPair{
		//役職とその役職の人の一覧をペアにしたもの
		const std::string profession;
		const std::vector<std::string> nameVec;
		CreditPair(const std::string &i_profession,const std::vector<std::string> &i_nameVec)
			:profession(i_profession),nameVec(i_nameVec){}
		~CreditPair()=default;
	};

	CreditScene();
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;//場面遷移をする時に、どこに移動するかを決める関数。この値の設定方法は、クラスごとに実装方法を変えて良い。

	//変数
	std::vector<CreditPair> m_creditList;
	int m_strY;//文字列描画位置(y)
	int m_strHeight;
	//フォント
	const int m_professionFont;
	const int m_nameFont;
	//背景
	const int m_backPic;
	//BGM
	const Resource::BGM m_bgm;
};

#endif // !DEF_CREDITSCENE_H
