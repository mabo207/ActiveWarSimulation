#ifndef DEF_PLAYERMOVESCENE_H
#define DEF_PLAYERMOVESCENE_H

#include"MoveScene.h"
#include"input.h"

//プレイヤーがユニットを動かす際に使う関数
class PlayerMoveScene:public MoveScene{
	//型・列挙体

	//定数

	//変数
protected:
	const MouseButtonUI m_waitButton,m_researchButton;
	Vector2D m_mousePosJustBefore;//直前フレームにおけるマウスの位置
	bool m_mouseLeftFlag;//プレイヤー操作場面でマウスの左クリックをし始めたかどうか

	//関数
protected:

	//仮想関数のオーバーライド
	Vector2D CalculateInputVec()const;

public:
	PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	
	//仮想関数のオーバーライド
	int thisCalculate();
	void thisDraw()const;
	void ReturnProcess();
};

#endif // !DEF_PLAYERMOVESCENE_H
#pragma once
