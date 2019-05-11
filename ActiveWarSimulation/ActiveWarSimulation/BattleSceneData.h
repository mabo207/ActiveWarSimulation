#ifndef DEF_BATTLESCENEDATA_H
#define DEF_BATTLESCENEDATA_H

#include<vector>
#include<set>
#include"Unit.h"
#include"Terrain.h"
#include"ToolsLib.h"
#include"ScoreObserver.h"
#include"LatticeBattleField.h"

//バトル場面で、各クラスに渡すゲーム全体で扱うデータを一括管理するクラス
struct BattleSceneData{
	//型・列挙体
public:
	//今どのモードでのプレイをしているか
	enum class PlayMode{
		e_normal//通常プレイ
		,e_tutorial//チュートリアルモード
	};

	//定数
protected:

public:
	static const Vector2D mapDrawSize;//マップ描画部分の大きさ
	static const Vector2D uiDrawSize;//UI描画部分の大きさ

	//変数
public:
	//ゲームに使用する変数
	std::vector<BattleObject *> m_field;//フィールドに存在するオブジェクト一覧(動的ポインタの配列)
	Unit *m_operateUnit;//現在操作できるユニット(静的ポインタ)
	std::vector<Unit *> m_unitList;//フィールド上に生存しているユニット一覧。OPの順にソートされている。要素は全てm_fieldに格納されており、先頭はm_operateUnitになる。1ユニットの行動終了のタイミングでソートする。

	//評価に使用する変数
	float m_totalOP;//今までに消費されたOPの合計値。ターン数を計測するのに使う。
	std::shared_ptr<ScoreObserver> m_scoreObserver;

	//描画に必要な変数
	std::shared_ptr<Terrain> m_mapRange;//マップ全体を表す線分(対角線)
	Vector2D m_stageSize;//ステージの大きさ(なお、ステージで一番左上にある点は(0,0)とする)

	//読み込みの情報
	const std::string m_stageName;

	//グラフィックデータ
	const int m_mapPic;//マップ全体のグラフィック
	const int m_turnTimerPic;//タイマーの絵
	static const size_t drawOrderHelpNum=2;
	int m_drawOrderHelp[drawOrderHelpNum];//オーダーの矢印分岐について、それぞれどんな意味かを描画する

	//サウンドデータ
	//bgm
	const int m_mapBGM;//マップBGM
	//effect
	const int m_aimchangeSound;//狙いが変わった時の音
	const int m_attackSound;//ダメージの音
	const int m_healSound;//回復の音
	const int m_footSound;//足音

	//その他の変数
	FpsMeasuring m_fpsMesuring;//fps計測器。タイマーの意味合いも兼ねる。
	const int m_orderFont;//オーダー表示の際のフォント
	const PlayMode m_playMode;//今のプレイモード
	

	//デバッグ用変数
public:
	bool m_drawObjectShapeFlag;//フィールドオブジェクトの当たり判定図形を描画するかどうか

	//関数
protected:
	BattleSceneData(const std::string &stagename,const PlayMode playMode);//継承クラス用コンストラクタ
	float CalculateOperateUnitFinishOP()const;//m_operateUnitが行動終了した際、opはいくらになるかを計算する関数(行動終了しても先頭ユニットであれば2番目になるまでOPを消費させる必要があるため)
	float CalculateOperateUnitFinishOP(float op)const;//OPの消費を踏まえた計算をできるようにするために、引数から計算する関数を用意した

public:
	BattleSceneData(const std::string &stagename);
	virtual ~BattleSceneData();
	void UpdateFix();//m_fieldのFix::Kindを更新する関数
	bool PositionUpdate(const Vector2D inputVec);//ユニットの位置を更新、m_operateUnitに移動操作がされればtrueを返す。
	void SortUnitList();//m_unitListをソートしなおす。
	void FinishUnitOperation();//次のユニットへの遷移処理
	Unit *GetUnitPointer(Vector2D pos)const;//pos(マップ上の座標)にいるユニットを返す。このユニットに攻撃する可能性がある事を考慮してconstはつけない。
	bool CanOperateUnitMove()const;//m_operateUnitが移動することが可能か（周りに何があるかは考えない）
	int CalculateTurn()const;
	std::shared_ptr<LatticeBattleField> CalculateLatticeBattleField()const;//現在のステージの状態の格子点認識情報を計算して返す。

	//情報描画関数
	void DrawField(const std::set<const BattleObject *> &notDraw={})const;//フィールドの描画、ユニットの描画は別。こいつより前に描画したものはマップ絵で全て消えるはず。
	void DrawUnit(bool infoDrawFlag,const std::set<const Unit *> &notDraw={})const;//ユニットの描画、情報表示UIを表示するかを設定できる
	void DrawHPGage()const;//全ユニットのHPゲージの描画
	void DrawOrder(const std::set<const BattleObject *> &lineDraw={})const;//ユニットのオーダー順番の描画。lineDrawに含まれるユニットは、必ずマップ上の位置とオーダーが線で結ばれる

	//静的関数
public:
	static bool JudgeMousePushInsideMapDrawZone(int mouseCode,bool continuousFlag);//マップ描画領域でmouseCodeを押したかどうか。また、continuousFlagをtrueにすればフレーム数が>0であるかどうかを、falseにすればフレーム数が==1であるかを判定する。

};


#endif // !DEF_BATTLESCENEDATA_H

#pragma once
