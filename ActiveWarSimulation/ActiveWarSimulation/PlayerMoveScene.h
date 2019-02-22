#ifndef DEF_PLAYERMOVESCENE_H
#define DEF_PLAYERMOVESCENE_H

#include"MoveScene.h"
#include"input.h"
#include<functional>
#include<array>

//プレイヤーがユニットを動かす際に使う関数
class PlayerMoveScene:public MoveScene{
	//型・列挙体

	//定数

	//変数
private:
	//継承先から関数を経由せずに変更して欲しくないので、private
	const MouseButtonUI m_waitButton,m_researchButton,m_menuButton;
	bool m_mouseLeftFlag;//プレイヤー操作場面でマウスの左クリックをし始めたかどうか
protected:
	Vector2D m_mousePosJustBefore;//直前フレームにおけるマウスの位置(これだけは継承先で変更しないといけないのでprotected)

	//関数
protected:
	//行動可能かの判定とその行動処理は、クラス外からアクセスしてほしくないのでprotected
	//virtual化、ダメ、ゼッタイ
	//判定の結果と、シーン遷移情報を返す
	std::pair<bool,int> AttackProcess();//攻撃遷移の判定と処理
	std::pair<bool,int> SkillProcess();//スキル使用遷移の判定と処理
	std::pair<bool,int> CounterclockwiseProcess();//狙いのキャラの変更(反時計回り)の判定と処理
	std::pair<bool,int> ClockwiseProcess();//狙いのキャラの変更(時計回り)の判定と処理
	std::pair<bool,int> MouseSetAimedUnitProcess();//マウスによる狙いのキャラの変更の判定と処理
	std::pair<bool,int> ItemProcess();//アイテム使用遷移の判定と処理
	std::pair<bool,int> WaitProcess();//待機遷移の判定と処理
	std::pair<bool,int> CancelProcess();//移動キャンセルの判定と処理
	std::pair<bool,int> ResearchProcess();//マップ調べモード遷移の判定と処理
	std::pair<bool,int> SystemMenuProcess();//システムメニュー遷移の判定と処理
	std::pair<bool,int> MoveProcess();//移動の判定と処理
	
	//仮想関数のオーバーライド
	Vector2D CalculateInputVec()const;

public:
	PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	virtual ~PlayerMoveScene(){}

	//仮想関数のオーバーライド
	virtual int thisCalculate();
	virtual void thisDraw()const;
	virtual void ReturnProcess();

	//静的メンバ
protected:
	static const std::array<std::function<std::pair<bool,int>(PlayerMoveScene&)>,11> inCalculateProcessFunction;//処理を簡単にするために行動可能かの判定とその行動処理へのポインタを保持

};

#endif // !DEF_PLAYERMOVESCENE_H
#pragma once
