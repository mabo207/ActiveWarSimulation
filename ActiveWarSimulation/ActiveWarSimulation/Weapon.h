#ifndef DEF_WEAPON_H
#define DEF_WEAPON_H

#include<string>
#include<map>
#include<memory>
#include"Unit.h"
#include"DamageCalculators.h"
class DamageCalculator;//�z�Q�Ƃ�h�����߂ɐ錾�݂̂���
class Unit;//�z�Q�Ƃ�h�����߂ɐ錾�݂̂���

//����ɂ��ẴN���X
class Weapon{
	//�񋓑́E�^
	struct AttackInfo{
		//�U�����ʂ��i�[����\����
		int damage;//�_���[�W
		//��Ԉُ�Ȃǂ�����΂�����ǉ����Ă���
	};

	//�萔

	//�ϐ�
protected:
	std::string m_name;//���O
	int m_power;//�З�
	float m_length;//�˒�
	float m_cost;//����OP
	std::shared_ptr<DamageCalculator> m_calculator;//�_���[�W�v�Z��

	//�֐�
protected:
	Weapon(const std::string &name,int power,float length,float cost,std::shared_ptr<DamageCalculator> calculator):m_name(name),m_power(power),m_length(length),m_cost(cost),m_calculator(calculator){}
public:
	~Weapon(){}

	std::string GetName()const{
		return m_name;
	}
	int GetPower()const{
		return m_power;
	}
	float GetLength()const{
		return m_length;
	}
	float GetCost()const{
		return m_cost;
	}

	AttackInfo GetAttackInfo(const Unit *attacker,const Unit *defender)const;

	//�ÓI�ϐ�
private:
	static const std::map<std::string,std::shared_ptr<Weapon>> m_weaponMap;//����̈ꗗ���i�[����ÓI�ϐ�
	
	//�ÓI�֐�
private:
	static std::map<std::string,std::shared_ptr<Weapon>> InitWeaponMap();
	static std::pair<std::string,std::shared_ptr<Weapon>> CreateWeaponMapElement(std::shared_ptr<Weapon> weapon);
public:
	static const std::shared_ptr<Weapon> GetWeapon(const std::string &name);
};

#endif // !DEF_WEAPON_H
#pragma once
