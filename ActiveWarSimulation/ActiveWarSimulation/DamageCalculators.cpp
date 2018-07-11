#include"DamageCalculators.h"
#include"Weapon.h"
#include<algorithm>
#pragma comment(lib, "winmm.lib")

//---------------PhysicalCalculator-----------------
PhysicalCalculator::PhysicalCalculator(double powerRate,double defRate,double weaponRate)
	:DamageCalculator(),m_powerRate(powerRate),m_defRate(defRate),m_weaponRate(weaponRate){}

PhysicalCalculator::~PhysicalCalculator(){}

int PhysicalCalculator::VCalculateDamage(const Unit *attacker,const Unit *defender)const{
	//「力」*倍率+「武器威力」*倍率-「守備」*倍率を求める。
	int pal=(int)(attacker->GetBaseStatus().power*m_powerRate)+(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate)-(int)(defender->GetBaseStatus().def*m_defRate);
	//0を下回るダメージは0に。
	if(pal<0){
		pal=0;
	}
	return pal;
}

bool PhysicalCalculator::VJudgeWeild(const Unit *attacker,const Unit *defender)const{
	//敵対関係であれば攻撃できる
	return !Unit::Team::JudgeFriend(attacker->GetBattleStatus().team,defender->GetBattleStatus().team);
}

//---------------MagicCalculator-----------------
MagicCalculator::MagicCalculator(double powerRate,double defRate,double weaponRate)
	:DamageCalculator(),m_powerRate(powerRate),m_defRate(defRate),m_weaponRate(weaponRate){}

MagicCalculator::~MagicCalculator(){}

int MagicCalculator::VCalculateDamage(const Unit *attacker,const Unit *defender)const{
	//「魔力」*倍率+「武器威力」*倍率-「魔防」*倍率を求める。
	int pal=(int)(attacker->GetBaseStatus().mpower*m_powerRate)+(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate)-(int)(defender->GetBaseStatus().mdef*m_defRate);
	//0を下回るダメージは0に。
	if(pal<0){
		pal=0;
	}
	return pal;
}

bool MagicCalculator::VJudgeWeild(const Unit *attacker,const Unit *defender)const{
	//敵対関係であれば攻撃できる
	return !Unit::Team::JudgeFriend(attacker->GetBattleStatus().team,defender->GetBattleStatus().team);
}

//---------------RecoverCalculator-----------------
RecoverCalculator::RecoverCalculator(double powerRate,double weaponRate)
	:DamageCalculator(),m_powerRate(powerRate),m_weaponRate(weaponRate){}

RecoverCalculator::~RecoverCalculator(){}

int RecoverCalculator::VCalculateDamage(const Unit *healer,const Unit *receiver)const{
	//「魔力」*倍率+「武器威力」*倍率で回復力は決定する。
	int pal=(int)(healer->GetBaseStatus().mpower*m_powerRate)+(int)(healer->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	//被回復者の最大HPを上回るダメージは上回らないように。また、ダメージとして認識させるために負の値にする。
	pal=-std::min(pal,receiver->GetBaseStatus().maxHP-receiver->GetBattleStatus().HP);
	return pal;
}

bool RecoverCalculator::VJudgeWeild(const Unit *attacker,const Unit *defender)const{
	//味方関係であれば攻撃できる
	return Unit::Team::JudgeFriend(attacker->GetBattleStatus().team,defender->GetBattleStatus().team);
}


