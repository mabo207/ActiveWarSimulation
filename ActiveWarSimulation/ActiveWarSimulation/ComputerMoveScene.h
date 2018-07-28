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
	std::vector<std::pair<size_t,Vector2D>> m_latticeRoute;//通る格子点(size_t:格子点番号(m_latticeInShape.size()をオーバーする場合は格子点ではないことを示す),Vector2D:その位置。誤差によって計算が位置から正しい格子点番号を求められない懸念があるのでこうした。)
	Unit *m_targetUnit;//狙おうとしているユニット

	std::vector<LatticeDistanceInfo> distvec;//デバッグ用距離可視化変数

	//関数
protected:
	Vector2D CalculateLatticePointPos(size_t x,size_t y)const;
	Vector2D CalculateLatticePointPos(size_t index)const;
	std::pair<std::pair<size_t,Vector2D>,Unit *> DecideTargetPoint(const std::vector<LatticeDistanceInfo> &distanceInfo)const;//目標とする格子点とその格子点に到着したあとに狙う位置を決める
	void CalculateLatticeDistanceInfo(std::vector<LatticeDistanceInfo> &latticeDistanceInfo)const;//各格子点までの距離と行き方を計算する関数。情報量が多いので情報の格納先を引数で渡す。
	void CalculateLatticeRoute();//m_latticeInShapeからm_latticeRouteを計算する
	void ImpassableLatticeInShape(const size_t index);//indexの格子点を通れないようにする(1にする)。尚且つその隣接点が0であれば2にする。
	void ImpassableLatticeInShape(const size_t x,const size_t y);
	float CalculateEvaluate(const Unit *punit,const std::vector<LatticeDistanceInfo> &distanceInfo)const;

	//仮想関数のオーバーライド
	Vector2D CalculateInputVec()const;

public:
	ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~ComputerMoveScene();

	//仮想関数のオーバーライド
	int thisCalculate();
	void thisDraw()const;

};

#endif // !DEF_COMPUTERMOVESCENE_H
#pragma once
