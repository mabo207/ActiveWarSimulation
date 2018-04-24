#ifndef DEF_COMPUTERMOVESCENE_H
#define DEF_COMPUTERMOVESCENE_H

#include"MoveScene.h"

//AIがユニットを動かす際に使う関数
class ComputerMoveScene:public MoveScene{
	//型・列挙体

	//定数

	//変数

	//関数
protected:

	//仮想関数のオーバーライド
	Vector2D CalculateInputVec()const;
};

#endif // !DEF_COMPUTERMOVESCENE_H
#pragma once
