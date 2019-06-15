#ifndef DEF_STAGESELECTSCENE_H
#define DEF_STAGESELECTSCENE_H

#include"GameScene.h"
#include<vector>
#include<string>
#include"input.h"
#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"

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
		std::shared_ptr<GameScene> CreateScene()const;
	};

	//定数

	//変数
protected:
	std::vector<StageInfoInStageSelect> m_stageInfoVec;//ステージ一覧情報
	NextSceneName m_nextSceneName;//次の場面は何か、VGetNextScene()で使用
	std::shared_ptr<BaseUIInStageSelect> m_ui;//現在のUI
	std::shared_ptr<BaseUIInStageSelect::ControledData> m_uiControledData;//UIが管理するデータ
	
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

