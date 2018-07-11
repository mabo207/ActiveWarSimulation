#include"DamageCalculators.h"
#include"Weapon.h"
#include<algorithm>
#pragma comment(lib, "winmm.lib")

//---------------PhysicalCalculator-----------------
PhysicalCalculator::PhysicalCalculator(double powerRate,double defRate,double weaponRate)
	:DamageCalculator(),m_powerRate(powerRate),m_defRate(defRate),m_weaponRate(weaponRate){}

PhysicalCalculator::~PhysicalCalculator(){}

int PhysicalCalculator::VCalculateDamage(const Unit *attacker,const Unit *defender)const{
	//�u�́v*�{��+�u����З́v*�{��-�u����v*�{�������߂�B
	int pal=(int)(attacker->GetBaseStatus().power*m_powerRate)+(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate)-(int)(defender->GetBaseStatus().def*m_defRate);
	//0�������_���[�W��0�ɁB
	if(pal<0){
		pal=0;
	}
	return pal;
}

bool PhysicalCalculator::VJudgeWeild(const Unit *attacker,const Unit *defender)const{
	//�G�Ί֌W�ł���΍U���ł���
	return !Unit::Team::JudgeFriend(attacker->GetBattleStatus().team,defender->GetBattleStatus().team);
}

//---------------MagicCalculator-----------------
MagicCalculator::MagicCalculator(double powerRate,double defRate,double weaponRate)
	:DamageCalculator(),m_powerRate(powerRate),m_defRate(defRate),m_weaponRate(weaponRate){}

MagicCalculator::~MagicCalculator(){}

int MagicCalculator::VCalculateDamage(const Unit *attacker,const Unit *defender)const{
	//�u���́v*�{��+�u����З́v*�{��-�u���h�v*�{�������߂�B
	int pal=(int)(attacker->GetBaseStatus().mpower*m_powerRate)+(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate)-(int)(defender->GetBaseStatus().mdef*m_defRate);
	//0�������_���[�W��0�ɁB
	if(pal<0){
		pal=0;
	}
	return pal;
}

bool MagicCalculator::VJudgeWeild(const Unit *attacker,const Unit *defender)const{
	//�G�Ί֌W�ł���΍U���ł���
	return !Unit::Team::JudgeFriend(attacker->GetBattleStatus().team,defender->GetBattleStatus().team);
}

//---------------RecoverCalculator-----------------
RecoverCalculator::RecoverCalculator(double powerRate,double weaponRate)
	:DamageCalculator(),m_powerRate(powerRate),m_weaponRate(weaponRate){}

RecoverCalculator::~RecoverCalculator(){}

int RecoverCalculator::VCalculateDamage(const Unit *healer,const Unit *receiver)const{
	//�u���́v*�{��+�u����З́v*�{���ŉ񕜗͂͌��肷��B
	int pal=(int)(healer->GetBaseStatus().mpower*m_powerRate)+(int)(healer->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	//��񕜎҂̍ő�HP������_���[�W�͏���Ȃ��悤�ɁB�܂��A�_���[�W�Ƃ��ĔF�������邽�߂ɕ��̒l�ɂ���B
	pal=-std::min(pal,receiver->GetBaseStatus().maxHP-receiver->GetBattleStatus().HP);
	return pal;
}

bool RecoverCalculator::VJudgeWeild(const Unit *attacker,const Unit *defender)const{
	//�����֌W�ł���΍U���ł���
	return Unit::Team::JudgeFriend(attacker->GetBattleStatus().team,defender->GetBattleStatus().team);
}


