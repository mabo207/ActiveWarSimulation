#ifndef DEF_WATCHLOGSCENE_H
#define DEF_WATCHLOGSCENE_H

#include"GameScene.h"
#include"BattleSceneData.h"

//ログを閲覧する場面
class WatchLogScene:public GameScene{
public:
	class WatchLogSceneFactory:public SceneFactory{
	public:
		WatchLogSceneFactory(const std::string &i_logFileName):logFileName(i_logFileName){}
		~WatchLogSceneFactory()=default;
		std::shared_ptr<GameScene> CreateIncompleteScene()const{
			return std::shared_ptr<GameScene>(new WatchLogScene(logFileName));
		}

	private:
		const std::string logFileName;
	};

	WatchLogScene(const std::string &logFileName);
	~WatchLogScene();
	void InitCompletely();//メインスレッド以外で行うことができる初期化処理群
	void Activate();//初期化が終了し、このSceneの動作が始まる時に行う処理。BGMの再生処理など。
	int Calculate();
	void Draw()const;

private:
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;//場面遷移をする時に、どこに移動するかを決める関数。この値の設定方法は、クラスごとに実装方法を変えて良い。
	void AdaptLog();

	size_t m_logIndex;
	std::shared_ptr<BattleSceneData> m_battleSceneData;
	std::vector<std::shared_ptr<LogElement>> m_logList;
};

#endif // !DEF_WATCHLOGSCENE_H
