#ifndef DEF_MOVESCENE_H
#define DEF_MOVESCENE_H

#include"BattleSceneData.h"
#include"GameScene.h"
#include"Unit.h"

//バトルの移動場面についての記述
//COMによる移動はこいつを継承して作ればラクそう
class MoveScene:public GameScene{
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

	//関数
protected:
	Vector2D CalculateInputVec()const;//味方ターン時は入力されてるアナログスティックのベクトルを返す。敵ターン時はAIに従って移動方向のベクトルを返す。
	bool PositionUpdate(const Vector2D inputVec);//ユニットの位置を更新、m_operateUnitに移動操作がされればtrueを返す。
	void FinishUnitOperation();//次のユニットへの遷移処理
	void SetAimedUnit(float angle=0.0f,int turntimes=1);//m_aimedUnitを変更する。turntimesだけ時計回りに回転させる
	void ProcessAttack();//攻撃処理を行う
	bool JudgeAttackCommandUsable()const;//攻撃コマンドを使用可能かどうか

public:
	MoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~MoveScene();
	int Calculate();
	void Draw()const;
};


#endif // !DEF_MOVESCENE_H


#pragma once
