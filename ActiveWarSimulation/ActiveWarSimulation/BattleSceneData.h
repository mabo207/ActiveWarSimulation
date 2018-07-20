#ifndef DEF_BATTLESCENEDATA_H
#define DEF_BATTLESCENEDATA_H

#include<vector>
#include<set>
#include"Unit.h"
#include"Terrain.h"
#include"ToolsLib.h"

//バトル場面で、各クラスに渡すゲーム全体で扱うデータを一括管理するクラス
struct BattleSceneData{
	//型・列挙体
public:

	//定数
protected:

public:

	//変数
public:
	//ゲームに使用する変数
	std::vector<BattleObject *> m_field;//フィールドに存在するオブジェクト一覧(動的ポインタの配列)
	Unit *m_operateUnit;//現在操作できるユニット(静的ポインタ)
	std::vector<Unit *> m_unitList;//フィールド上に生存しているユニット一覧。OPの順にソートされている。要素は全てm_fieldに格納されており、先頭はm_operateUnitになる。1ユニットの行動終了のタイミングでソートする。

	//グラフィック関連の変数
	std::shared_ptr<Terrain> m_Window;//ウインドウ全体を表す線分(対角線)
	Vector2D m_stageSize;//ステージの大きさ(なお、ステージで一番左上にある点は(0,0)とする)

	//その他の変数
	FpsMeasuring m_fpsMesuring;//fps計測器。タイマーの意味合いも兼ねる。
	const int m_orderFont;//オーダー表示の際のフォント
	
	//関数
protected:
	float CalculateOperateUnitFinishOP()const;//m_operateUnitが行動終了した際、opはいくらになるかを計算する関数(行動終了しても先頭ユニットであれば2番目になるまでOPを消費させる必要があるため)
	float CalculateOperateUnitFinishOP(float op)const;//OPの消費を踏まえた計算をできるようにするために、引数から計算する関数を用意した

public:
	BattleSceneData(const char *stagename);
	~BattleSceneData();
	void UpdateFix();//m_fieldのFix::Kindを更新する関数
	bool PositionUpdate(const Vector2D inputVec);//ユニットの位置を更新、m_operateUnitに移動操作がされればtrueを返す。
	void SortUnitList();//m_unitListをソートしなおす。
	void FinishUnitOperation();//次のユニットへの遷移処理
	Unit *GetUnitPointer(Vector2D pos)const;//pos(マップ上の座標)にいるユニットを返す。このユニットに攻撃する可能性がある事を考慮してconstはつけない。

	//情報描画関数
	void DrawField(const std::set<const BattleObject *> &notDraw={})const;//フィールドの描画、ユニットの描画は別
	void DrawUnit(bool infoDrawFlag,const std::set<const Unit *> &notDraw={})const;//ユニットの描画、情報表示UIを表示するかを設定できる
	void DrawHPGage()const;//全ユニットのHPゲージの描画
	void DrawOrder()const;//ユニットのオーダー順番の描画
};


#endif // !DEF_BATTLESCENEDATA_H

#pragma once
