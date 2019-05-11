#ifndef DEF_COMPUTERMOVESCENE_H
#define DEF_COMPUTERMOVESCENE_H

#include"MoveScene.h"
#include<vector>

//AIがユニットを動かす際に使う関数
class ComputerMoveScene:public MoveScene{
	//型・列挙体
protected:
	//攻撃が当たり判定押し出しによって上手くいかなかった場合の情報管理
	struct AttackFailedInfo{
		//変数
		size_t m_count;//失敗した回数
		size_t m_frame;//前回失敗から経過したフレーム数
		//関数
		AttackFailedInfo();
		~AttackFailedInfo(){}
		bool JudgeAttackProcessProceed()const;//攻撃処理の分岐に進ませて大丈夫か
		bool JudgeRetry()const;//移動をやり直させるか
		void RetryProcess();//やり直し処理
	};

	//定数
protected:

	//変数
protected:
	//格子点の定義のための情報
	const std::shared_ptr<LatticeBattleField> m_latticeField;

	//AIのための情報
	std::vector<std::pair<size_t,Vector2D>> m_latticeRoute;//通る格子点(size_t:格子点番号(m_latticeInShape.size()をオーバーする場合は格子点ではないことを示す),Vector2D:その位置。誤差によって計算が位置から正しい格子点番号を求められない懸念があるのでこうした。)
	Unit *m_targetUnit;//狙おうとしているユニット
	bool m_actionWaiting;//行動を行うまでの待ち時間を待っている状態か？（対象変更した瞬間に攻撃だと分かりづらそう）
	int m_nextScene;//どの行動場面へ進むか(thisCalculate()の返り値が何か)
	AttackFailedInfo m_attackFailedInfo;//押し出し処理による攻撃の失敗のリカバリーのための情報

	//その他の情報
	size_t m_aimChangeFrame;//狙いを変更してから経った時間
	size_t m_waitingFrame;//待機状態のフレーム数

	std::vector<LatticeBattleField::LatticeDistanceInfo> distvec;//デバッグ用距離可視化変数

	//関数
protected:
	std::pair<std::pair<size_t,Vector2D>,Unit *> DecideTargetPoint(const std::vector<LatticeBattleField::LatticeDistanceInfo> &distanceInfo)const;//目標とする格子点とその格子点に到着したあとに狙う位置を決める
	void CalculateLatticeRoute();//m_latticeInShapeからm_latticeRouteを計算する。行動を再計算する場合にも用いる。
	float CalculateEvaluate(const Unit *punit,const std::vector<LatticeBattleField::LatticeDistanceInfo> &distanceInfo)const;
	float CalculateActionEvaluate(const Unit *punit)const;//行動した際の影響度についての評価値、CalculateEvaluate()の他にも、その場待機するかどうかの判定に用いることがあるので分離。
	int BranchingWaitingProcess();//AI行動中にその場待機することになった時、その場で攻撃できるキャラがいるなら攻撃するように処理を分岐するための関数

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
