#define NOMINMAX//std::min���g����悤�ɂ��邽��
#include"DamageCalculators.h"
#include"DxLib.h"
#include"ToolsLib.h"
#include"Weapon.h"
#include<algorithm>

//---------------DamageCalculator-----------------
void DamageCalculator::VDrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//1�s�ڂ�"DAMAGE"�A2�s�ڂɃ_���[�W�ʂ�`�悷��
	const int fontsize[2]={GetFontSizeToHandle(explainFont),GetFontSizeToHandle(numberFont)};
	int height=0;
	for(size_t i=0;i<2;i++){
		height+=fontsize[i];
	}
	int dy=-height/2;
	DrawStringCenterBaseToHandle(centerX,centerY+dy,"DAMAGE",GetColor(255,255,255),explainFont,false,GetColor(0,0,0));
	dy+=fontsize[0];
	DrawStringCenterBaseToHandle(centerX,centerY+dy,std::to_string(VCalculateDamage(attacker,defender)).c_str(),GetColor(192,64,64),numberFont,false,GetColor(0,0,0));
}

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

void RecoverCalculator::VDrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//1�s�ڂ�"RECOVER"�A2�s�ڂɉ񕜗ʂ�`�悷��
	const int fontsize[2]={GetFontSizeToHandle(explainFont),GetFontSizeToHandle(numberFont)};
	int height=0;
	for(size_t i=0;i<2;i++){
		height+=fontsize[i];
	}
	int dy=-height/2;
	DrawStringCenterBaseToHandle(centerX,centerY+dy,"RECOVER",GetColor(255,255,255),explainFont,false,GetColor(0,0,0));
	dy+=fontsize[0];
	DrawStringCenterBaseToHandle(centerX,centerY+dy,std::to_string(-VCalculateDamage(attacker,defender)).c_str(),GetColor(64,192,64),numberFont,false,GetColor(0,0,0));
}

