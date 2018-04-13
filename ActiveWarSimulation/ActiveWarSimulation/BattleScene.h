#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"

//バトルのゲームプレイ場面全般の管理を行うクラス
class BattleScene:public GameScene{
	//型・列挙体
public:
	
	//定数
protected:
	
public:

	//変数
protected:
	//ゲームの進行を管理する変数
	std::shared_ptr<BattleSceneElement> m_sceneData;//一番基底となるシーン

	//ゲームに使用する変数
	std::shared_ptr<BattleSceneData> m_battleSceneData;//ゲーム内全体で使うデータ
	
	//関数
protected:

public:
	BattleScene(const char *stagename);
	~BattleScene();
	int Calculate();
	void Draw()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
