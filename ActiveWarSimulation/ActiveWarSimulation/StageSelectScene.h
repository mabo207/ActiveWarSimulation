#ifndef DEF_STAGESELECTSCENE_H
#define DEF_STAGESELECTSCENE_H

#include"GameScene.h"
#include<vector>
#include<string>
#include<map>
#include"input.h"
#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"
#include"BGM.h"

class StageSelectScene:public GameScene{
	//型・列挙体
private:
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
		std::shared_ptr<GameScene> CreateIncompleteScene()const;
	};

	//定数

	//変数
protected:
	std::vector<StageInfoInStageSelect> m_stageInfoVec;//ステージ一覧情報
	NextSceneName m_nextSceneName;//次の場面は何か、VGetNextScene()で使用
	std::shared_ptr<BaseUIInStageSelect> m_ui;//現在のUI
	std::shared_ptr<BaseUIInStageSelect::ControledData> m_uiControledData;//UIが管理するデータ

	//画像
	const int m_backDefaultPic;//背景（デフォルト）
	const int m_backNightPic;//背景（夜）
	const int m_backMorningPic;//背景（朝焼け）

	//ボタン
	const MouseButtonUI m_backButton;

	//フォント
	const int m_stageNameFont;
	const int m_explainFont;
	
	//BGM
	const Resource::BGM m_bgm;

	//効果音

private:
	std::map<std::string,int> m_stageInfoFactoryMap;//ステージ情報のロードのための情報(ミニマップについては別スレッド処理をするためにStageInfoInStageSelectクラスで直接ロードせずにこの変数にロードする)

	//関数
protected:
	StageSelectScene();
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;
	void DrawBack()const;//背景の描画

public:
	~StageSelectScene();
	void InitCompletely();
	void Activate();
	int Calculate();
	void Draw()const;
};

#endif // !DEF_STAGESELECTSCENE_H

