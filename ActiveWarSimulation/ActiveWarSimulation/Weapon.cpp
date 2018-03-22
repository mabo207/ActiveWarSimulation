#include"Weapon.h"

//---------------Weapon----------------
const std::map<std::string,std::shared_ptr<Weapon>> Weapon::m_weaponMap=Weapon::InitWeaponMap();

std::pair<std::string,std::shared_ptr<Weapon>> Weapon::CreateWeaponMapElement(std::shared_ptr<Weapon> weapon){
	return std::pair<std::string,std::shared_ptr<Weapon>>(weapon->GetName(),weapon);
}

std::map<std::string,std::shared_ptr<Weapon>> Weapon::InitWeaponMap(){
	std::map<std::string,std::shared_ptr<Weapon>> map;
	//�ȉ��񋓂���map�Ɋi�[���Ă���
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�S�̑�",Unit::closeAttackLength,50.0f))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�S�̋|",Unit::openAttackLength,50.0f))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�t�@�C�A�[",Unit::openAttackLength,50.0f))));
	//map��Ԃ�
	return map;
}

const std::shared_ptr<Weapon> Weapon::GetWeapon(const std::string &name){
	std::map<std::string,std::shared_ptr<Weapon>>::const_iterator it=m_weaponMap.find(name);
	return it->second;
}
