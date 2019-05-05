#ifndef DEF_WEAPON_H
#define DEF_WEAPON_H

#include<string>
#include<map>
#include<memory>
#include"Unit.h"
#include"DamageCalculators.h"
class DamageCalculator;//zŠÂQÆ‚ğ–h‚®‚½‚ß‚ÉéŒ¾‚Ì‚İ‚·‚é
enum class WeaponActionKind;//zŠÂQÆ‚ğ–h‚®‚½‚ß‚ÉéŒ¾‚Ì‚İ‚·‚é
class Unit;//zŠÂQÆ‚ğ–h‚®‚½‚ß‚ÉéŒ¾‚Ì‚İ‚·‚é

//•Ší‚É‚Â‚¢‚Ä‚ÌƒNƒ‰ƒX
class Weapon{
	//—ñ‹“‘ÌEŒ^
public:
	struct AttackInfo{
		//UŒ‚Œ‹‰Ê‚ğŠi”[‚·‚é\‘¢‘Ì
		int damage;//ƒ_ƒ[ƒW
		//ó‘ÔˆÙí‚È‚Ç‚ª‚ ‚ê‚Î‚»‚ê‚à’Ç‰Á‚µ‚Ä‚¢‚­
	};

	//’è”
public:
	static const float closeAttackLength;//‹ßÚUŒ‚‚ÌË’ö
	static const float openAttackLength;//ŠÔÚUŒ‚‚ÌË’ö
	static const float longAttackLength;//‰“‹——£UŒ‚‚ÌË’ö

	//•Ï”
protected:
	std::string m_name;//–¼‘O
	int m_power;//ˆĞ—Í
	float m_length;//Ë’ö
	float m_cost;//Á”ïOP
	std::shared_ptr<DamageCalculator> m_calculator;//ƒ_ƒ[ƒWŒvZ®
	std::string m_explain;//à–¾•¶

	//ŠÖ”
protected:
	Weapon(const std::string &name,int power,float length,float cost,std::shared_ptr<DamageCalculator> calculator,const std::string explain):m_name(name),m_power(power),m_length(length),m_cost(cost),m_calculator(calculator),m_explain(explain){}
public:
	~Weapon(){}

	std::string GetName()const{
		return m_name;
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
	WeaponActionKind GetActionKind()const;

	AttackInfo GetAttackInfo(const Unit *attacker,const Unit *defender)const;
	bool JudgeWeild(const Unit *attacker,const Unit *defender)const;//attacker¨defender‚Ö‚Ì’ÊíUŒ‚‚ğÀs‰Â”\‚Å‚ ‚é‚©
	std::string GetEffectivenessString(const Unit *attacker)const;//attacker‚ª‚±‚Ì•Ší‚ğg‚¤‚Æ‚Ç‚ñ‚ÈŒø‰Ê‚ğ“¾‚ç‚ê‚é‚©
	void DrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const;//s“®—\‘z‚ğ•`‰æ‚·‚é

	//Ã“I•Ï”
private:
	static const std::map<std::string,std::shared_ptr<Weapon>> m_weaponMap;//•Ší‚Ìˆê——‚ğŠi”[‚·‚éÃ“I•Ï”
	
	//Ã“IŠÖ”
private:
	static std::map<std::string,std::shared_ptr<Weapon>> InitWeaponMap();
	static std::pair<std::string,std::shared_ptr<Weapon>> CreateWeaponMapElement(std::shared_ptr<Weapon> weapon);
public:
	static const std::shared_ptr<Weapon> GetWeapon(const std::string &name);
};

#endif // !DEF_WEAPON_H
#pragma once
