#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include<vector>
#include"GameScene.h"
#include"Unit.h"
#include"Terrain.h"

//バトル場面表すクラス
class BattleScene:public GameScene{
	//型・列挙体
public:

	//定数
protected:

public:

	//変数
protected:
	//ゲームの進行を管理する変数


	//ゲームに使用する変数
	std::vector<BattleObject *> m_field;//フィールドに存在するオブジェクト一覧(動的ポインタの配列)
	Unit *m_operateUnit;//現在操作できるユニット(静的ポインタ)

	//グラフィック関連の変数
	std::shared_ptr<Terrain> m_Window;//ウインドウ全体を表す線分(対角線)


	//関数
protected:
	void UpdateFix();
	bool PositionUpdate();//ユニットの位置を更新、m_operateUnitに移動操作がされればtrueを返す。

public:
	BattleScene(const char *stagename);
	~BattleScene();
	int Calculate();
	void Draw()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
