#include"DamageCalculators.h"
#include"Weapon.h"

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


