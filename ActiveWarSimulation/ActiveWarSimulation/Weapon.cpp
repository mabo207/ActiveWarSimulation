#include"Weapon.h"

//---------------Weapon----------------
Weapon::AttackInfo Weapon::GetAttackInfo(const Unit *attacker,const Unit *defender)const{
	AttackInfo info;
	//Šeî•ñ‚ÌŒvZ
	info.damage=m_calculator->VCalculateDamage(attacker,defender);
	//Œ‹‰Ê‚ğ•Ô‚·
	return info;
}

//ˆÈ‰ºÃ“I•Ï”
const std::map<std::string,std::shared_ptr<Weapon>> Weapon::m_weaponMap=Weapon::InitWeaponMap();

std::pair<std::string,std::shared_ptr<Weapon>> Weapon::CreateWeaponMapElement(std::shared_ptr<Weapon> weapon){
	return std::pair<std::string,std::shared_ptr<Weapon>>(weapon->GetName(),weapon);
}

std::map<std::string,std::shared_ptr<Weapon>> Weapon::InitWeaponMap(){
	std::map<std::string,std::shared_ptr<Weapon>> map;
	//ˆÈ‰º—ñ‹“‚µ‚Ämap‚ÉŠi”[‚µ‚Ä‚¢‚­
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("“S‚Ì‘„",5,Unit::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator())))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("“S‚Ì‹|",2,Unit::openAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator())))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("ƒtƒ@ƒCƒA[",5,Unit::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator())))));
	//map‚ğ•Ô‚·
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
