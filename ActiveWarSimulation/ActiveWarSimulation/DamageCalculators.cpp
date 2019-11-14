#define NOMINMAX//std::minを使えるようにするため
#include"DamageCalculators.h"
#include"DxLib.h"
#include"ToolsLib.h"
#include"Weapon.h"
#include"Unit.h"
#include<algorithm>

//---------------DamageCalculator-----------------
void DamageCalculator::VDrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//1行目に"DAMAGE"、2行目にダメージ量を描画する
	const int fontsize[2]={GetFontSizeToHandle(explainFont),GetFontSizeToHandle(numberFont)};
	int height=0;
	for(size_t i=0;i<2;i++){
		height+=fontsize[i];
	}
	int dy=-height/2;
	DrawStringCenterBaseToHandle(centerX,centerY+dy,"DAMAGE",GetColor(255,255,255),explainFont,false,GetColor(0,0,0));
	dy+=fontsize[0];
	DrawStringCenterBaseToHandle(centerX,centerY+dy,std::to_string(VCalculateDamage(attacker,defender)).c_str(),GetColor(240,68,48),numberFont,false,GetColor(0,0,0));
}

void DamageCalculator::VDrawExtendPredict(const int centerX,const int centerY,const double exRate,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//1行目に"DAMAGE"、2行目にダメージ量を描画する
	const int fontsize[2]={(int)(GetFontSizeToHandle(explainFont)*exRate),(int)(GetFontSizeToHandle(numberFont)*exRate)};
	const std::string str[2]={"DAMAGE",std::to_string(VCalculateDamage(attacker,defender))};
	const int font[2]={explainFont,numberFont};
	const unsigned int color[2]={GetColor(255,255,255),GetColor(240,68,48)};
	int wholeHeight=0;
	for(size_t i=0;i<2;i++){
		wholeHeight+=fontsize[i];
	}
	int dy=-wholeHeight/2;
	for(size_t i=0;i<2;i++){
		const int width=(int)(GetDrawStringWidthToHandle(str[i].c_str(),str[i].size(),font[i])*exRate);
		DrawExtendStringToHandle(centerX-width/2,centerY+dy,exRate,exRate,str[i].c_str(),color[i],font[i],GetColor(0,0,0));
		dy+=fontsize[i];
	}
}

//---------------PhysicalCalculator-----------------
PhysicalCalculator::PhysicalCalculator(double powerRate,double defRate,double weaponRate)
	:DamageCalculator(Kind::e_physicalAttack),m_powerRate(powerRate),m_defRate(defRate),m_weaponRate(weaponRate){}

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
	//敵対関係かつ同一ユニットでなければ攻撃できる
	return (!Unit::Team::JudgeFriend(attacker->GetBattleStatus().team,defender->GetBattleStatus().team)) && (attacker!=defender);
}

std::string PhysicalCalculator::VGetPowerString(const Unit *attacker)const{
	const int weaponPower=(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	const int totalPower=(int)(attacker->GetBaseStatus().power*m_powerRate)+(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	return "武器威力："+std::to_string(weaponPower)+"　合計威力："+std::to_string(totalPower);
}

//---------------MagicCalculator-----------------
MagicCalculator::MagicCalculator(double powerRate,double defRate,double weaponRate)
	:DamageCalculator(Kind::e_magicAttack),m_powerRate(powerRate),m_defRate(defRate),m_weaponRate(weaponRate){}

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
	//敵対関係かつ同一ユニットでなければ攻撃できる
	return (!Unit::Team::JudgeFriend(attacker->GetBattleStatus().team,defender->GetBattleStatus().team)) && (attacker!=defender);
}

std::string MagicCalculator::VGetPowerString(const Unit *attacker)const{
	const int weaponPower=(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	const int totalPower=(int)(attacker->GetBaseStatus().mpower*m_powerRate)+(int)(attacker->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	return "武器威力："+std::to_string(weaponPower)+"　合計威力："+std::to_string(totalPower);
}

//---------------RecoverCalculator-----------------
RecoverCalculator::RecoverCalculator(double powerRate,double weaponRate)
	:DamageCalculator(Kind::e_recover),m_powerRate(powerRate),m_weaponRate(weaponRate){}

RecoverCalculator::~RecoverCalculator(){}

int RecoverCalculator::VCalculateDamage(const Unit *healer,const Unit *receiver)const{
	//「魔力」*倍率+「武器威力」*倍率で回復力は決定する。
	int pal=(int)(healer->GetBaseStatus().mpower*m_powerRate)+(int)(healer->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	//被回復者の最大HPを上回るダメージは上回らないように。また、ダメージとして認識させるために負の値にする。
	pal=-std::min(pal,receiver->GetBaseStatus().maxHP-receiver->GetBattleStatus().HP);
	return pal;
}

bool RecoverCalculator::VJudgeWeild(const Unit *healer,const Unit *defender)const{
	//味方関係かつ同一ユニットでなければ回復できる
	return Unit::Team::JudgeFriend(healer->GetBattleStatus().team,defender->GetBattleStatus().team) && (healer!=defender);
}

std::string RecoverCalculator::VGetPowerString(const Unit *healer)const{
	const int weaponPower=(int)(healer->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	const int totalPower=(int)(healer->GetBaseStatus().mpower*m_powerRate)+(int)(healer->GetBattleStatus().weapon->GetPower()*m_weaponRate);
	return "武器回復量："+std::to_string(weaponPower)+"　合計回復量："+std::to_string(totalPower);
}

void RecoverCalculator::VDrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//1行目に"RECOVER"、2行目に回復量を描画する
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

void RecoverCalculator::VDrawExtendPredict(const int centerX,const int centerY,const double exRate,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//1行目に"DAMAGE"、2行目にダメージ量を描画する
	const int fontsize[2]={(int)(GetFontSizeToHandle(explainFont)*exRate),(int)(GetFontSizeToHandle(numberFont)*exRate)};
	const std::string str[2]={"RECOVER",std::to_string(-VCalculateDamage(attacker,defender))};
	const int font[2]={explainFont,numberFont};
	const unsigned int color[2]={GetColor(255,255,255),GetColor(64,192,64)};
	int wholeHeight=0;
	for(size_t i=0;i<2;i++){
		wholeHeight+=fontsize[i];
	}
	int dy=-wholeHeight/2;
	for(size_t i=0;i<2;i++){
		const int width=(int)(GetDrawStringWidthToHandle(str[i].c_str(),str[i].size(),font[i])*exRate);
		DrawExtendStringToHandle(centerX-width/2,centerY+dy,exRate,exRate,str[i].c_str(),color[i],font[i],GetColor(0,0,0));
		dy+=fontsize[i];
	}
}

