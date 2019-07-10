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
	//�e���̌v�Z
	info.damage=m_calculator->VCalculateDamage(attacker,defender);
	//���ʂ�Ԃ�
	return info;
}

bool Weapon::JudgeWeild(const Unit *attacker,const Unit *defender)const{
	return m_calculator->VJudgeWeild(attacker,defender);
}

std::string Weapon::GetEffectivenessString(const Unit *attacker)const{
	//1�s�ڂɕ���U���͂ƍ��v�U���́i���ʂɂ���ĕ��ʂ��قȂ�j�A2�s�ڂɎ˒��ƃR�X�g���o�͂���B
	std::string ret;
	ret+=m_calculator->VGetPowerString(attacker);
	ret+="\n";
	ret+="�˒��F"+std::to_string((int)m_length)+"�@�R�X�g�F"+std::to_string((int)m_cost);
	return ret;
}

void Weapon::DrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//DamageCalculator�ɓ�����
	m_calculator->VDrawPredict(centerX,centerY,explainFont,numberFont,attacker,defender);
}

//�ȉ��ÓI�ϐ�
const std::map<std::string,std::shared_ptr<Weapon>> Weapon::m_weaponMap=Weapon::InitWeaponMap();

std::pair<std::string,std::shared_ptr<Weapon>> Weapon::CreateWeaponMapElement(const std::shared_ptr<Weapon> &weapon){
	return std::pair<std::string,std::shared_ptr<Weapon>>(weapon->GetResisterName(),weapon);
}

std::map<std::string,std::shared_ptr<Weapon>> Weapon::InitWeaponMap(){
	std::map<std::string,std::shared_ptr<Weapon>> map;
	//�ȉ��񋓂���map�Ɋi�[���Ă���
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("���̌�","sword_bronze",5,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�ቿ�i�ȓ���p�̌��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�S�̌�","sword_iron",6,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�N�ł������₷���W���I�Ȍ��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�|�̌�","sword_steel",7,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"���������Â炢���З͂̂��錕�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("��̌�","sword_silver",8,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�����Ő؂ꖡ�o�c�O���Ȍ��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("���̑�","lance_bronze",5,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�ቿ�i�ȓ���p�̑��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�S�̑�","lance_iron",6,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�N�ł������₷���W���I�ȑ��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�|�̑�","lance_steel",7,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"���������Â炢���З͂̂��鑄�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("��̑�","lance_silver",8,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�����łƂĂ��s���ȑ��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("���̋|","bow_bronze",3,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�ቿ�i�ȓ���p�̋|�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�S�̋|","bow_iron",4,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�N�ł������₷���W���I�ȋ|�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�|�̋|","bow_steel",5,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"���������Â炢���З͂̂���|�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("��̋|","bow_silver",6,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�����͂����������ȋ|�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�t�@�C�A�[�̏�","book_fire",5,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"���̊�{���l�܂�������̖������B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�t���C���̏�","book_flame",6,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"�N�ł������₷�����̖������B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�u���C�Y�̏�","book_blaze",7,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"�����ǂނ̂�������̖������B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�{���K�m�̏�","book_volcano",8,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"���̃G�L�X�p�[�g�����̖������B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�q�[���̏�","rod_heal",5,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"������Ƃ�������Ɏg����s�̂̏�B\n�y������ʁz�s���J�n���ɑ����L������HP��1��",AttackInfo(-1)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("���X�g�̏�","rod_rest",7,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"���΂ɂ���l�ɖ�����^�����B\n�y������ʁz�s���J�n���ɑ����L������HP��1��",AttackInfo(-1)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�L���A�̏�","rod_cure",9,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"�񕜂̗͂����߂�s�v�c�ȏ�B\n�y������ʁz�s���J�n���ɑ����L������HP��2��",AttackInfo(-2)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("���J�o�[�̏�","rod_recover",11,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"�P�K����������ƕ]���̏�B\n�y������ʁz�s���J�n���ɑ����L������HP��2��",AttackInfo(-2)))));
	//map��Ԃ�
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

