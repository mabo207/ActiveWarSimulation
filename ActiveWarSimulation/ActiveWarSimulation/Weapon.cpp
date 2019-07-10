#include"Weapon.h"
#include"Unit.h"

//---------------Weapon----------------
const float Weapon::closeAttackLength=Unit::rivalInpenetratableCircleSize*1.15f;
const float Weapon::openAttackLength=Weapon::closeAttackLength*2.0f;
const float Weapon::longAttackLength=Weapon::closeAttackLength*3.0f;

DamageCalculator::Kind Weapon::GetActionKind()const{
	return m_calculator->GetKind();
}

Weapon::AttackInfo Weapon::GetAttackInfo(const Unit *attacker,const Unit *defender)const{
	AttackInfo info;
	//各情報の計算
	info.damage=m_calculator->VCalculateDamage(attacker,defender);
	//結果を返す
	return info;
}

bool Weapon::JudgeWeild(const Unit *attacker,const Unit *defender)const{
	return m_calculator->VJudgeWeild(attacker,defender);
}

std::string Weapon::GetEffectivenessString(const Unit *attacker)const{
	//1行目に武器攻撃力と合計攻撃力（効果によって文面が異なる）、2行目に射程とコストを出力する。
	std::string ret;
	ret+=m_calculator->VGetPowerString(attacker);
	ret+="\n";
	ret+="射程："+std::to_string((int)m_length)+"　コスト："+std::to_string((int)m_cost);
	return ret;
}

void Weapon::DrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//DamageCalculatorに投げる
	m_calculator->VDrawPredict(centerX,centerY,explainFont,numberFont,attacker,defender);
}

//以下静的変数
const std::map<std::string,std::shared_ptr<Weapon>> Weapon::m_weaponMap=Weapon::InitWeaponMap();

std::pair<std::string,std::shared_ptr<Weapon>> Weapon::CreateWeaponMapElement(std::shared_ptr<Weapon> weapon){
	return std::pair<std::string,std::shared_ptr<Weapon>>(weapon->GetName(),weapon);
}

std::map<std::string,std::shared_ptr<Weapon>> Weapon::InitWeaponMap(){
	std::map<std::string,std::shared_ptr<Weapon>> map;
	//以下列挙してmapに格納していく
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鉄の剣",5,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"低価格で扱いやすい標準的な剣。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鉄の槍",5,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"低価格で扱いやすい標準的な槍。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鉄の弓",3,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"低価格で扱いやすい標準的な弓。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("ファイアーの書",5,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"魔道の基本が詰まっている標準的な魔法。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("ヒールの杖",5,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"回復の力を強める標準的な杖。\n【特殊効果】行動開始時に装備キャラのHPを1回復",AttackInfo(-1)))));
	//mapを返す
	return map;
}

const std::shared_ptr<Weapon> Weapon::GetWeapon(const std::string &name){
	std::map<std::string,std::shared_ptr<Weapon>>::const_iterator it=m_weaponMap.find(name);
	if(it!=m_weaponMap.end()){
		return it->second;
	} else{
		return nullptr;
	}
}

