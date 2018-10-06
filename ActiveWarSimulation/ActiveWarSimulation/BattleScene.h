#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"

//バトルのゲームプレイ場面全般の管理を行うクラス
class BattleScene:public MainControledGameScene{
	//型・列挙体
public:
	struct RequiredInfoToMakeBattleScene:public RequiredInfoToMakeClass{
		std::string m_stagename;
		RequiredInfoToMakeBattleScene(const std::string &stagename):m_stagename(stagename){}
		Kind GetKind()const{
			return e_battleScene;
		}
	};

	//定数
protected:
	static const int resetInterval;

public:

	//変数
protected:
	//ステージやり直し処理関連
	int m_resetFlame;//リセット処理にかかっている時間
	bool m_resetFlag;//リセット処理中かどうか

	//ゲームの進行を管理する変数
	std::shared_ptr<BattleSceneElement> m_sceneData;//一番基底となるシーン

	//ゲームに使用する変数
	std::shared_ptr<BattleSceneData> m_battleSceneData;//ゲーム内全体で使うデータ
	
	//関数
protected:
	virtual std::shared_ptr<BattleSceneElement> VGetSwitchUnitScene()const;//SwitchUnitSceneかDemoSwitchUnitSceneのどっちを使うか

public:
	BattleScene(const char *stagename);
	virtual ~BattleScene();
	int Calculate();
	void Draw()const;
	std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
