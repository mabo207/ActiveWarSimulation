#ifndef DEF_WEAPON_H
#define DEF_WEAPON_H

#include<string>
#include<map>
#include<memory>
#include"Unit.h"

//����ɂ��ẴN���X
class Weapon{
	//�񋓑́E�^
	
	//�萔

	//�ϐ�
protected:
	std::string m_name;//���O
	float m_length;//�˒�
	float m_cost;//����OP

	//�֐�
protected:
	Weapon(const std::string &name,float length,float cost):m_name(name),m_length(length),m_cost(cost){}
public:
	~Weapon(){}

	std::string GetName()const{
		return m_name;
	}
	float GetLength()const{
		return m_length;
	}
	float GetCost()const{
		return m_cost;
	}

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
