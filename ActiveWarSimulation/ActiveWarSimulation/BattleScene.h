#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"

//バトルのゲームプレイ場面全般の管理を行うクラス
class BattleScene:public MainControledGameScene{
	//型・列挙体
public:
	class BattleSceneFactory:public MainSceneFactory{
	public:
		BattleSceneFactory(const std::string &stageDirName,const std::string &title,const int level);
		~BattleSceneFactory();
		std::shared_ptr<MainControledGameScene> CreateScene()const;
	private:
		const std::string m_stageDirName;
		const std::string m_title;
		const int m_level;
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
	BattleScene(const std::string &stageDirName,const std::string &titleName,const int stageLevel);
	virtual std::shared_ptr<BattleSceneElement> VGetSwitchUnitScene()const;//SwitchUnitSceneかDemoSwitchUnitSceneのどっちを使うか
	void ResetGame();

public:
	virtual ~BattleScene();
	virtual int Calculate();
	virtual void Draw()const;
	std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
