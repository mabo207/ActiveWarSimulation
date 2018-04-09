#ifndef DEF_ATTACKSCENE_H
#define DEF_ATTACKSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include<memory>

//ユニットを動かすゲーム場面
class AttackScene:public BattleSceneElement{
	//型・列挙体
public:


	//定数
protected:
	static const int motionFlame;//アニメーションする全体時間
	static const int damageBeginFlame,damageEndFlame;//ダメージ文字が出現するタイミング、アニメーションが終わるタイミング

	//変数
protected:
	//ゲームのデータ
	std::shared_ptr<BattleSceneData> m_battleSceneData;

	//攻撃されるユニットのデータ
	Unit *m_aimedUnit;

	//位置変更データ
	PositionComplexControl m_attackMotion;//攻撃ユニット
	PositionControl m_damageMotion;//ダメージ文字

	//攻撃情報についてのデータ
	Weapon::AttackInfo m_attackinfo;//攻撃情報
	int m_damageFont;//ダメージ描画に用いるフォント

	//関数
protected:

	//仮想関数のオーバーライド
	int thisCalculate();
	void thisDraw()const;
	int UpdateNextScene(int index);

public:
	AttackScene(std::shared_ptr<BattleSceneData> data,Unit *aimedUnit);
	~AttackScene();


};

#endif // !DEF_ATTACKSCENE_H


#pragma once
