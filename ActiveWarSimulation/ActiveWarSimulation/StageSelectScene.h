#ifndef DEF_STAGESELECTSCENE_H
#define DEF_STAGESELECTSCENE_H

#include"GameScene.h"
#include<vector>
#include<string>
#include"input.h"
#include"ScoreRankingData.h"
#include"BaseUIInStageSelect.h"

class StageSelectScene:public GameScene{
	//型・列挙体
private:
	//ステージ情報
	struct StageInfo{
		//変数
		int m_mapPic;//マップグラフィック(縮小表示)(push_back()の際にデストラクタが呼ばれグラフィックが消されるので、削除はデストラクタでは行わない。どうしてもデストラクタでしたくなったら、コピーコンストラクタを作って再度CopyGraph()をしよう。)
		std::string m_dirName;//ディレクトリ名
		std::string m_titleName;//ステージ名
		std::string m_explain;//ステージ説明文
		ScoreRankingData::StageScoreData m_rankingVec;//ランキングデータ
		int m_x,m_y;//ボタンの位置

		//関数
		StageInfo(const int mapPic,const std::string &dirName,const std::string &explain,const ScoreRankingData &rankingData);
		~StageInfo();
	};
	//次の場面を何にするか
	enum class NextSceneName{
		e_title
		,e_battle
	};

public:
	class StageSelectSceneFactory:public SceneFactory{
		//クラスを作るのに特に必要なデータはない
	public:
		StageSelectSceneFactory():SceneFactory(){}
		~StageSelectSceneFactory(){}
		std::shared_ptr<GameScene> CreateScene()const;
	};

	//定数

	//変数
protected:
	std::vector<StageInfo> m_stageInfoVec;//ステージ一覧情報
	NextSceneName m_nextSceneName;//次の場面は何か、VGetNextScene()で使用
	std::shared_ptr<BaseUIInStageSelect> m_ui;//現在のUI
	std::shared_ptr<BaseUIInStageSelect::ControledData> m_uiControledData;//UIが管理するデータ
	
	//マウスでクリックできるボタン群
	const MouseButtonUI m_beforeStageButton;
	const MouseButtonUI m_afterStageButton;
	const MouseButtonUI m_backButton;
	const MouseButtonUI m_playButton;

	//グラフィック等
	const int m_backPic;
	const int m_stageNameFont;
	const int m_explainFont;
	
	//効果音

	//関数
protected:
	StageSelectScene();
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;

public:
	~StageSelectScene();
	int Calculate();
	void Draw()const;
};

#endif // !DEF_STAGESELECTSCENE_H

