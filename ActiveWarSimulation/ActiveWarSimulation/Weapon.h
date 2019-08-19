#ifndef DEF_WEAPON_H
#define DEF_WEAPON_H

#include<string>
#include<map>
#include<memory>
#include<vector>
#include"DamageCalculators.h"
class Unit;//循環参照を防ぐために宣言のみする

//武器についてのクラス
class Weapon{
	//列挙体・型
public:
	struct AttackInfo{
		//攻撃結果を格納する構造体
		int damage;//ダメージ
		//状態異常などがあればそれも追加していく
		explicit AttackInfo(int i_damage=0)
			:damage(i_damage)
		{}
		~AttackInfo()=default;
	};
	enum class Kind{
		e_sword
		,e_lance
		,e_bow
		,e_book
		,e_rod
	};

	//定数
public:
	static const float closeAttackLength;//近接攻撃の射程
	static const float openAttackLength;//間接攻撃の射程
	static const float longAttackLength;//遠距離攻撃の射程

	//変数
protected:
	std::string m_name;//名前
	std::string m_resisterName;//登録名
	Kind m_kind;//武器の種類
	int m_power;//威力
	float m_length;//射程
	float m_cost;//消費OP
	std::shared_ptr<DamageCalculator> m_calculator;//ダメージ計算式
	std::string m_explain;//説明文
	AttackInfo m_turnStartDamage;//ターン開始時のダメージ処理

	//関数
protected:
	Weapon(const std::string &name
		,const std::string &resisterName
		,Kind kind
		,int power
		,float length
		,float cost
		,std::shared_ptr<DamageCalculator> calculator
		,const std::string &explain
		,const AttackInfo &turnStartDamage
	)
		:m_name(name)
		,m_resisterName(resisterName)
		,m_kind(kind)
		,m_power(power)
		,m_length(length)
		,m_cost(cost)
		,m_calculator(calculator)
		,m_explain(explain)
		,m_turnStartDamage(turnStartDamage)
	{}
public:
	~Weapon(){}

	std::string GetName()const{
		return m_name;
	}
	std::string GetResisterName()const{
		return m_resisterName;
	}
	Kind GetKind()const{
		return m_kind;
	}
	int GetPower()const{
		return m_power;
	}
	float GetLength()const{
		return m_length;
	}
	float GetCost()const{
		return m_cost;
	}
	std::string GetExplain()const{
		return m_explain;
	}
	AttackInfo GetTurnStartDamage()const{
		return m_turnStartDamage;
	}
	DamageCalculator::Kind GetActionKind()const;

	AttackInfo GetAttackInfo(const Unit *attacker,const Unit *defender)const;
	bool JudgeWeild(const Unit *attacker,const Unit *defender)const;//attacker→defenderへの通常攻撃を実行可能であるか
	std::string GetEffectivenessString(const Unit *attacker)const;//attackerがこの武器を使うとどんな効果を得られるか
	void DrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const;//行動予想を描画する

	//静的変数
private:
	static const std::map<std::string,std::shared_ptr<Weapon>> s_weaponMap;//武器の一覧を格納する静的変数
	
	//静的関数
private:
	static std::map<std::string,std::shared_ptr<Weapon>> InitWeaponMap();
	static std::pair<std::string,std::shared_ptr<Weapon>> CreateWeaponMapElement(const std::shared_ptr<Weapon> &weapon);
public:
	static const std::shared_ptr<Weapon> GetWeapon(const std::string &name);
	static std::vector<std::shared_ptr<Weapon>> GetKindVecSorted(const Kind weaponKind);//特定の武器種を「コスト→射程→威力」でソートして返す
};

#endif // !DEF_WEAPON_H
#pragma once
