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
	//�G�Ί֌W�����ꃆ�j�b�g�łȂ���΍U���ł���
	return (!Unit::Team::JudgeFriend(attacker->GetBattleStatus().team,defender->GetBattleStatus().team)) && (attacker!=defender);
}

std::string PhysicalCalculator::VGetPowerString(const Unit *attacker)const{
	const int weaponPower=(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	const int totalPower=(int)(attacker->GetBaseStatus().power*m_powerRate)+(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	return "����З́F"+std::to_string(weaponPower)+"�@���v�З́F"+std::to_string(totalPower);
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
	//�G�Ί֌W�����ꃆ�j�b�g�łȂ���΍U���ł���
	return (!Unit::Team::JudgeFriend(attacker->GetBattleStatus().team,defender->GetBattleStatus().team)) && (attacker!=defender);
}

std::string MagicCalculator::VGetPowerString(const Unit *attacker)const{
	const int weaponPower=(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	const int totalPower=(int)(attacker->GetBaseStatus().mpower*m_powerRate)+(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	return "����З́F"+std::to_string(weaponPower)+"�@���v�З́F"+std::to_string(totalPower);
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

bool RecoverCalculator::VJudgeWeild(const Unit *healer,const Unit *defender)const{
	//�����֌W�����ꃆ�j�b�g�łȂ���Ή񕜂ł���
	return Unit::Team::JudgeFriend(healer->GetBattleStatus().team,defender->GetBattleStatus().team) && (healer!=defender);
}

std::string RecoverCalculator::VGetPowerString(const Unit *healer)const{
	const int weaponPower=(int)(healer->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	const int totalPower=(int)(healer->GetBaseStatus().mpower*m_powerRate)+(int)(healer->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	return "����񕜗ʁF"+std::to_string(weaponPower)+"�@���v�񕜗ʁF"+std::to_string(totalPower);
}


