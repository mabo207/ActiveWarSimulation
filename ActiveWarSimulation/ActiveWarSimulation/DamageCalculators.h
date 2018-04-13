#ifndef DEF_DAMAGECALCULATORS_H
#define DEF_DAMAGECALCULATORS_H

#include"Unit.h"
class Unit;//循環参照を防ぐために宣言のみする

//ダメージの計算式回り
//基底クラス
class DamageCalculator{
	//関数
protected:
	DamageCalculator(){}
	virtual ~DamageCalculator(){}
public:
	virtual int VCalculateDamage(const Unit *attacker,const Unit *defender)const=0;//ダメージを計算する関数
	virtual bool VJudgeWeild(const Unit *attacker,const Unit *defender)const=0;//attacker→defenderへの通常攻撃を実行可能であるか
};

//物理攻撃
class PhysicalCalculator:public DamageCalculator{
	//変数
protected:
	double m_powerRate;//ユニットの「力」にかかる倍率
	double m_defRate;//ユニットの「守備」にかかる倍率
	double m_weaponRate;//武器の「威力」にかかる倍率

	//関数
public:
	PhysicalCalculator(double powerRate=1.0,double defRate=1.0,double weaponRate=1.0);
	~PhysicalCalculator();
	int VCalculateDamage(const Unit *attacker,const Unit *defender)const;
	bool VJudgeWeild(const Unit *attacker,const Unit *defender)const;//attacker→defenderへの通常攻撃を実行可能であるか
};

//魔法攻撃
class MagicCalculator:public DamageCalculator{
	//変数
protected:
	double m_powerRate;//ユニットの「魔力」にかかる倍率
	double m_defRate;//ユニットの「魔防」にかかる倍率
	double m_weaponRate;//武器の「威力」にかかる倍率

	//関数
public:
	MagicCalculator(double powerRate=1.0,double defRate=1.0,double weaponRate=1.0);
	~MagicCalculator();
	int VCalculateDamage(const Unit *attacker,const Unit *defender)const;
	bool VJudgeWeild(const Unit *attacker,const Unit *defender)const;//attacker→defenderへの通常攻撃を実行可能であるか
};

//回復魔法
class RecoverCalculator:public DamageCalculator{
	//変数
protected:
	double m_powerRate;//ユニットの「魔力」にかかる倍率
	double m_weaponRate;//武器の「威力」にかかる倍率

	//関数
public:
	RecoverCalculator(double powerRate=1.0,double weaponRate=1.0);
	~RecoverCalculator();
	int VCalculateDamage(const Unit *healer,const Unit *receiver)const;
	bool VJudgeWeild(const Unit *attacker,const Unit *defender)const;//attacker→defenderへの通常攻撃を実行可能であるか
};

#endif // !DEF_DAMAGECALCULATORS_H
#pragma once
