#include"Weapon.h"

//---------------Weapon----------------
Weapon::AttackInfo Weapon::GetAttackInfo(const Unit *attacker,const Unit *defender)const{
	AttackInfo info;
	//各情報の計算
	info.damage=m_calculator->VCalculateDamage(attacker,defender);
	//結果を返す
	return info;
}

//以下静的変数
const std::map<std::string,std::shared_ptr<Weapon>> Weapon::m_weaponMap=Weapon::InitWeaponMap();

std::pair<std::string,std::shared_ptr<Weapon>> Weapon::CreateWeaponMapElement(std::shared_ptr<Weapon> weapon){
	return std::pair<std::string,std::shared_ptr<Weapon>>(weapon->GetName(),weapon);
}

std::map<std::string,std::shared_ptr<Weapon>> Weapon::InitWeaponMap(){
	std::map<std::string,std::shared_ptr<Weapon>> map;
	//以下列挙してmapに格納していく
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鉄の槍",5,Unit::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator())))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鉄の弓",2,Unit::openAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator())))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("ファイアー",5,Unit::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator())))));
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
