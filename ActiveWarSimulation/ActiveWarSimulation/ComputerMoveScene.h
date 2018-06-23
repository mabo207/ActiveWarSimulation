#ifndef DEF_COMPUTERMOVESCENE_H
#define DEF_COMPUTERMOVESCENE_H

#include"MoveScene.h"
#include<vector>

//AIがユニットを動かす際に使う関数
class ComputerMoveScene:public MoveScene{
	//型・列挙体

	//定数

	//変数
protected:
	std::vector<Vector2D> m_route;//通る格子点

	//関数
protected:

	//仮想関数のオーバーライド
	Vector2D CalculateInputVec()const;

public:
	ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);

	//仮想関数のオーバーライド
	int thisCalculate();

};

#endif // !DEF_COMPUTERMOVESCENE_H
#pragma once
