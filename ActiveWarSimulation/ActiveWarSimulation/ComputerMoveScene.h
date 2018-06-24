#ifndef DEF_COMPUTERMOVESCENE_H
#define DEF_COMPUTERMOVESCENE_H

#include"MoveScene.h"
#include<vector>

//AIがユニットを動かす際に使う関数
class ComputerMoveScene:public MoveScene{
	//型・列挙体
protected:
	//距離マップ作成時に用いる
	struct LatticeDistanceInfo{
		size_t index;//格子点番号
		size_t from;//どの格子点から到達できるか(大小関係、同値関係に用いない)
		float dist;//indexの格子点までの推定距離
		LatticeDistanceInfo(size_t i_index,size_t i_from,float i_dist):index(i_index),from(i_from),dist(i_dist){}
		bool operator<(const LatticeDistanceInfo &otherobj)const;
		bool operator==(const LatticeDistanceInfo &otherobj)const;
	};

	//定数
protected:
	static const size_t squareSize;//格子点の間隔。ユニットの大きさ以上でないと通れない場所を通ろうとするバグが発生する恐れがある

	//変数
protected:
	//格子点の定義のための情報
	std::vector<int> m_latticeInShape;//すべての格子点において、ユニットが通れる場所かどうかを格納している。0:通れる 1:図形内で通れない 2:隣接格子点が1なので通れない可能性がある
	const size_t m_xLatticeNum,m_yLatticeNum;//格子点が横・縦に何個存在するか。ステージの外にあっても動作としては問題ない。計算を楽に行うために用いる。

	//AIのための情報
	std::vector<Vector2D> m_route;//通る格子点

	//関数
protected:

	//仮想関数のオーバーライド
	Vector2D CalculateInputVec()const;

public:
	ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);

	//仮想関数のオーバーライド
	int thisCalculate();
	void thisDraw()const;

};

#endif // !DEF_COMPUTERMOVESCENE_H
#pragma once
