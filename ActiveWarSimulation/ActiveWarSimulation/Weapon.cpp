#include"Weapon.h"
#include"Unit.h"
#include<algorithm>

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

void Weapon::DrawExtendPredict(const int centerX,const int centerY,const double exRate,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//DamageCalculator�ɓ�����
	m_calculator->VDrawExtendPredict(centerX,centerY,exRate,explainFont,numberFont,attacker,defender);
}

//�ȉ��ÓI�ϐ�
const std::map<std::string,std::shared_ptr<Weapon>> Weapon::s_weaponMap=Weapon::InitWeaponMap();

std::pair<std::string,std::shared_ptr<Weapon>> Weapon::CreateWeaponMapElement(const std::shared_ptr<Weapon> &weapon){
	return std::pair<std::string,std::shared_ptr<Weapon>>(weapon->GetResisterName(),weapon);
}

std::map<std::string,std::shared_ptr<Weapon>> Weapon::InitWeaponMap(){
	std::map<std::string,std::shared_ptr<Weapon>> map;
	//�ȉ��񋓂���map�Ɋi�[���Ă���
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("���̌�","sword_bronze",Kind::e_sword,5,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�ቿ�i�ȓ���p�̌��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�S�̌�","sword_iron",Kind::e_sword,6,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�N�ł������₷���W���I�Ȍ��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�|�̌�","sword_steel",Kind::e_sword,7,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"���������Â炢���З͂̂��錕�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("��̌�","sword_silver",Kind::e_sword,8,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�����Ő؂ꖡ�o�c�O���Ȍ��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("���̑�","lance_bronze",Kind::e_lance,5,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�ቿ�i�ȓ���p�̑��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�S�̑�","lance_iron",Kind::e_lance,6,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�N�ł������₷���W���I�ȑ��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�|�̑�","lance_steel",Kind::e_lance,7,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"���������Â炢���З͂̂��鑄�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("��̑�","lance_silver",Kind::e_lance,8,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�����łƂĂ��s���ȑ��B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("���̋|","bow_bronze",Kind::e_bow,3,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�ቿ�i�ȓ���p�̋|�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�S�̋|","bow_iron",Kind::e_bow,4,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�N�ł������₷���W���I�ȋ|�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�|�̋|","bow_steel",Kind::e_bow,5,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"���������Â炢���З͂̂���|�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("��̋|","bow_silver",Kind::e_bow,6,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"�����͂����������ȋ|�B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�t�@�C�A�[�̏�","book_fire",Kind::e_book,5,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"���̊�{���l�܂�������̖������B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�t���C���̏�","book_flame",Kind::e_book,6,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"�N�ł������₷�����̖������B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�u���C�Y�̏�","book_blaze",Kind::e_book,7,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"�����ǂނ̂�������̖������B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�{���K�m�̏�","book_volcano",Kind::e_book,8,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"���̃G�L�X�p�[�g�����̖������B",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�q�[���̏�","rod_heal",Kind::e_rod,5,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"������Ƃ�������Ɏg����s�̂̏�B\n�y������ʁz�s���J�n���ɑ����L������HP��1��",AttackInfo(-1)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("���X�g�̏�","rod_rest",Kind::e_rod,7,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"���΂ɂ���l�ɖ�����^�����B\n�y������ʁz�s���J�n���ɑ����L������HP��1��",AttackInfo(-1)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("�L���A�̏�","rod_cure",Kind::e_rod,9,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"�񕜂̗͂����߂�s�v�c�ȏ�B\n�y������ʁz�s���J�n���ɑ����L������HP��2��",AttackInfo(-2)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("���J�o�[�̏�","rod_recover",Kind::e_rod,11,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"�P�K����������ƕ]���̏�B\n�y������ʁz�s���J�n���ɑ����L������HP��2��",AttackInfo(-2)))));
	//map��Ԃ�
	return map;
}

const std::shared_ptr<Weapon> Weapon::GetWeapon(const std::string &name){
	std::map<std::string,std::shared_ptr<Weapon>>::const_iterator it=s_weaponMap.find(name);
	if(it!=s_weaponMap.end()){
		return it->second;
	} else{
		return nullptr;
	}
}

std::vector<std::shared_ptr<Weapon>> Weapon::GetKindVecSorted(const Kind weaponKind){
	//����̕����̕����T��
	std::vector<std::shared_ptr<Weapon>> vec;
	for(std::map<std::string,std::shared_ptr<Weapon>>::const_iterator it=s_weaponMap.begin(),ite=s_weaponMap.end();it!=ite;it++){
		if(it->second->GetKind()==weaponKind){
			vec.push_back(it->second);
		}
	}
	//�\�[�g����
	const auto compare=[](const std::shared_ptr<Weapon> &left,const std::shared_ptr<Weapon> &right){
		//�R�X�g�̔�r
		const float leftCost=left->GetCost(),rightCost=right->GetCost();
		if(leftCost<rightCost){
			return true;
		} else if(leftCost>rightCost){
			return false;
		}
		//�˒��̔�r
		const float leftLength=left->GetLength(),rightLength=right->GetLength();
		if(leftLength<rightLength){
			return true;
		} else if(leftLength>rightLength){
			return false;
		}
		//�Ō�ɈЗ͂̔�r
		return (left->GetPower()<right->GetPower());
	};
	std::sort(vec.begin(),vec.end(),compare);
	//�Ԃ�
	return vec;
}
