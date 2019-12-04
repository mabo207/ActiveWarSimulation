#ifndef DEF_LATTICEBATTLEFIELD_H
#define DEF_LATTICEBATTLEFIELD_H

//ステージを格子点として捉えたもの
//BattleSceneDataを経由して使う
#include<vector>
#include<memory>
#include"Unit.h"

class LatticeBattleField{
public:
	//距離マップ作成時に用いる
	struct LatticeDistanceInfo{
		size_t index;//格子点番号
		size_t from;//どの格子点から到達できるか(大小関係、同値関係に用いない)
		float dist;//indexの格子点までの推定距離
		LatticeDistanceInfo(size_t i_index,size_t i_from,float i_dist):index(i_index),from(i_from),dist(i_dist){}
		bool operator<(const LatticeDistanceInfo &otherobj)const;
		bool operator==(const LatticeDistanceInfo &otherobj)const;
	};
	//その格子点が通れる点かどうか
	enum class LatticePass{
		e_passable				//通れる
		,e_unpassable			//通れない
		,e_neighborUnpassable	//隣接格子点が通れないので通れるか分からない
	};

	static std::shared_ptr<LatticeBattleField> Create(const std::vector<BattleObject *> &field,const Vector2D stageSize,const Unit * const punit,bool unitExist);
	virtual ~LatticeBattleField();
	size_t GetXLatticeNum()const{
		return m_xLatticeNum;
	}
	size_t GetYLatticeNum()const{
		return m_yLatticeNum;
	}
	LatticePass GetLatticeInShapeAt(size_t index)const;
	size_t GetLatticeInShapeSize()const{
		return m_latticeInShape.size();
	}
	void BecomeImpassibleLattice(size_t index);//m_latticeInShape[index]をe_unpassableにして、その周辺の格子点に対する処理もする
	void BecomeImpassibleLattice(size_t x,size_t y);
	void BecomeImpassibleLattice(const Unit *punit,const Unit::Team::Kind operateTeam);//Unit内部を通過不能にする。Unit以外のobjectは操作ユニット内部か外部かの判定が必要なので、BattleObjectに拡張することはしない。専らログ分析用。
	void CalculateLatticeDistanceInfo(std::vector<LatticeDistanceInfo> &retPal,const Vector2D startPos)const;//m_latticeInShapeを元に、各格子点とスタート地点までの距離・ルートに関する情報を返す
	Vector2D CalculateLatticePointPos(size_t x,size_t y)const;
	Vector2D CalculateLatticePointPos(size_t index)const;
	std::vector<float> CalculateRouteDistance(const Vector2D start,const std::vector<Vector2D> &endVec)const;//格子点情報を用いた迂回を考慮した経路距離を返す。距離マップを何回も作らないで済むように、目標地点をまとめて渡す。
	std::vector<float> CalculateRouteDistance(const std::vector<LatticeDistanceInfo> &distanceInfoVec,const std::vector<Vector2D> &endVec)const;//距離マップが既に作成されている時の経路距離計算。

	const static size_t latticeIntervalSize;//格子点は正方形状に並んでいるので、その大きさ

private:
	LatticeBattleField(const std::vector<BattleObject *> &field,const Vector2D stageSize,const Unit * const punit,bool unitExist);
	void CalculateLatticeInShape(const std::vector<BattleObject *> &field,const Unit * const punit,bool unitExist);//unitExistがfalseの時、ユニットがいないものとして距離計算をする

	std::vector<LatticePass> m_latticeInShape;
	const size_t m_xLatticeNum;
	const size_t m_yLatticeNum;
};

#endif // !DEF_LATTICEBATTLEFIELD_H