#ifndef DEF_PLAYERMOVESCENE_H
#define DEF_PLAYERMOVESCENE_H

#include"MoveScene.h"

//プレイヤーがユニットを動かす際に使う関数
class PlayerMoveScene:public MoveScene{
	//型・列挙体

	//定数

	//変数

	//関数
protected:

	//仮想関数のオーバーライド
	Vector2D CalculateInputVec()const;
public:
	PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);

};

#endif // !DEF_PLAYERMOVESCENE_H
#pragma once
