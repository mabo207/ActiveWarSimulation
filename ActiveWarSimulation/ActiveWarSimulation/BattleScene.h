#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include<vector>
#include"GameScene.h"
#include"Unit.h"
#include"Terrain.h"
#include"ToolsLib.h"

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
	std::vector<Unit *> m_unitList;//フィールド上に生存しているユニット一覧。OPの順にソートされている。要素は全てm_fieldに格納されており、先頭はm_operateUnitになる。1ユニットの行動終了のタイミングでソートする。
	Unit *m_aimedUnit;//現在の攻撃対象ユニット(静的ポインタ)

	//グラフィック関連の変数
	std::shared_ptr<Terrain> m_Window;//ウインドウ全体を表す線分(対角線)

	//その他の変数
	FpsMeasuring m_fpsMesuring;//fps計測器。タイマーの意味合いも兼ねる。

	//関数
protected:
	void UpdateFix();
	Vector2D CalculateInputVec()const;//味方ターン時は入力されてるアナログスティックのベクトルを返す。敵ターン時はAIに従って移動方向のベクトルを返す。
	bool PositionUpdate(const Vector2D inputVec);//ユニットの位置を更新、m_operateUnitに移動操作がされればtrueを返す。
	void SortUnitList();//m_unitListをソートしなおす。
	void FinishUnitOperation();//次のユニットへの遷移処理
	void SetAimedUnit(float angle=0.0f,int turntimes=1);//m_aimedUnitを変更する。turntimesだけ時計回りに回転させる
	void ProcessAttack();//攻撃処理を行う

public:
	BattleScene(const char *stagename);
	~BattleScene();
	int Calculate();
	void Draw()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
