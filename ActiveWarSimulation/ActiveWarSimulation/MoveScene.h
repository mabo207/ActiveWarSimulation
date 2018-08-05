#ifndef DEF_MOVESCENE_H
#define DEF_MOVESCENE_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"
#include"Unit.h"

//バトルの移動場面についての記述
//COMによる移動はこいつを継承して作ればラクそう
class MoveScene:public BattleSceneElement{
	//型・列挙体
public:
	struct RouteInfo{
		const Vector2D pos;
		const float OP;
		RouteInfo(Vector2D i_pos,float i_OP):pos(i_pos),OP(i_OP){}
	};
	
	//定数
protected:
	static const float routeFrequency;//OPをどのくらい消費するたびにm_routeに記録するか

public:

	//変数
protected:
	//ゲームの進行を管理する変数
	

	//ゲームに使用する変数
	std::shared_ptr<BattleSceneData> m_battleSceneData;//ゲーム内全体で使うデータ

	//ユニット移動場面でのみ使用する変数
	Unit *m_aimedUnit;//現在の攻撃対象ユニット(静的ポインタ)
	std::vector<RouteInfo> m_route;//操作ユニットが通った経路

	//グラフィック関連
	static const size_t attackedCursorPicNum=4;//分割数
	int m_attackedCursor[attackedCursorPicNum];//m_aimedUnitの上に乗せるカーソルの描画
	int m_operatedCursor[attackedCursorPicNum];//m_operatedUnitの上に乗せるカーソルの描画

	//関数
protected:
	bool PositionUpdate(const Vector2D inputVec);//ユニットの位置を更新、m_operateUnitに移動操作がされればtrueを返す。
	void FinishUnitOperation();//次のユニットへの遷移処理
	void SetAimedUnit(int turntimes);//m_aimedUnitとm_operatedUnitの位置からangleを自動的に求めるSetAimedUnit
	void SetAimedUnit(float angle,int turntimes);//m_aimedUnitを変更する。turntimesだけ時計回りに回転させる
	bool JudgeAttackCommandUsable()const;//攻撃コマンドを使用可能かどうか
	bool JudgeBecomeAimedUnit(const Unit *punit)const;//punitはm_aimedUnitになることが出来るかどうかを判定する

	//仮想関数のオーバーライド
	//int thisCalculate();//プレイヤーか敵かで異なるので
	virtual void thisDraw()const;//AIのデバッグのための描画ができるようにする
	int UpdateNextScene(int index);
	void ReturnProcess();

	//仮想関数
	virtual Vector2D CalculateInputVec()const=0;//味方ターン時は入力されてるアナログスティックのベクトルを返す。敵ターン時はAIに従って移動方向のベクトルを返す。

public:
	MoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	virtual ~MoveScene();
};


#endif // !DEF_MOVESCENE_H


#pragma once
