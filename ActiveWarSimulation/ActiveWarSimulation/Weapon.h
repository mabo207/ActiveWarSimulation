#ifndef DEF_WEAPON_H
#define DEF_WEAPON_H

#include<string>
#include<map>
#include<memory>
#include"Unit.h"

//武器についてのクラス
class Weapon{
	//列挙体・型
	
	//定数

	//変数
protected:
	std::string m_name;//名前
	float m_length;//射程
	float m_cost;//消費OP

	//関数
protected:
	Weapon(const std::string &name,float length,float cost):m_name(name),m_length(length),m_cost(cost){}
public:
	~Weapon(){}

	std::string GetName()const{
		return m_name;
	}
	float GetLength()const{
		return m_length;
	}
	float GetCost()const{
		return m_cost;
	}

	//静的変数
private:
	static const std::map<std::string,std::shared_ptr<Weapon>> m_weaponMap;//武器の一覧を格納する静的変数
	
	//静的関数
private:
	static std::map<std::string,std::shared_ptr<Weapon>> InitWeaponMap();
	static std::pair<std::string,std::shared_ptr<Weapon>> CreateWeaponMapElement(std::shared_ptr<Weapon> weapon);
public:
	static const std::shared_ptr<Weapon> GetWeapon(const std::string &name);
};

#endif // !DEF_WEAPON_H
#pragma once
