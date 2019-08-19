#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include"BattleSceneData.h"
#include"GameScene.h"
#include"BattleSceneElement.h"

//バトルのゲームプレイ場面全般の管理を行うクラス
class BattleScene:public GameScene{
	//型・列挙体
public:
	class BattleSceneFactory:public SceneFactory{
	public:
		BattleSceneFactory(const std::string &stageDirName,const std::string &title,const StageLevel level);
		~BattleSceneFactory();
		std::shared_ptr<GameScene> CreateIncompleteScene()const;
	private:
		const std::string m_stageDirName;
		const std::string m_title;
		const StageLevel m_level;
	};

	//定数
protected:
	static const int resetInterval;

public:

	//変数
protected:
	//ステージやり直し処理関連
	int m_resetFrame;//リセット処理にかかっている時間
	bool m_resetFlag;//リセット処理中かどうか

	//ゲームの進行を管理する変数
	std::shared_ptr<BattleSceneElement> m_sceneData;//一番基底となるシーン

	//ゲームに使用する変数
	std::shared_ptr<BattleSceneData> m_battleSceneData;//ゲーム内全体で使うデータ
	
	//関数
protected:
	BattleScene(std::shared_ptr<BattleSceneData> battleSceneData);//継承クラス用コンストラクタ
	BattleScene(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel);
	virtual std::shared_ptr<BattleSceneElement> VGetSwitchUnitScene()const;//SwitchUnitSceneかDemoSwitchUnitSceneのどっちを使うか
	void ResetGame();
	virtual std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;//チュートリアルとデモと通常プレイで戻る場所が違う

public:
	virtual ~BattleScene();
	virtual void InitCompletely();
	virtual void Activate();
	virtual int Calculate();
	virtual void Draw()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
