#ifndef DEF_ATTACKSCENE_H
#define DEF_ATTACKSCENE_H

#include"DamageScene.h"
#include<memory>

//ユニットを動かすゲーム場面
class AttackScene:public DamageScene{
	//型・列挙体
public:


	//定数
protected:
	static const float moveLength;//アニメーション時に移動する距離

	//変数
protected:
	//位置変更データ
	PositionComplexControl m_attackMotion;//攻撃ユニットのアニメーション

	//エフェクト
	int m_effect[30];

	//関数
protected:
	//仮想関数のオーバーライド
	void AnimationUpdate();//攻撃ユニットのアニメーション処理を追加
	void DrawUnit()const;//攻撃ユニットもアニメーションして表示
	void DrawAnimation()const;//エフェクトを表示する必要がある
	bool JudgeAnimationEnd()const;//攻撃ユニットのアニメーション処理も終了判定に加える
	void DamageProcess();//m_operatedUnitのOP削減を行ってからダメージ処理をする
	int UpdateNextScene(int index);
	void ReturnProcess();

public:
	AttackScene(const std::shared_ptr<BattleSceneData> &data,Unit *aimedUnit);
	~AttackScene();
};

#endif // !DEF_ATTACKSCENE_H


#pragma once
