#ifndef DEF_WEAPON_H
#define DEF_WEAPON_H

#include<string>
#include<map>
#include<memory>
#include"Unit.h"
#include"DamageCalculators.h"
class DamageCalculator;//�z�Q�Ƃ�h�����߂ɐ錾�݂̂���
enum class WeaponActionKind;//�z�Q�Ƃ�h�����߂ɐ錾�݂̂���
class Unit;//�z�Q�Ƃ�h�����߂ɐ錾�݂̂���

//����ɂ��ẴN���X
class Weapon{
	//�񋓑́E�^
public:
	struct AttackInfo{
		//�U�����ʂ��i�[����\����
		int damage;//�_���[�W
		//��Ԉُ�Ȃǂ�����΂�����ǉ����Ă���
	};

	//�萔
public:
	static const float closeAttackLength;//�ߐڍU���̎˒�
	static const float openAttackLength;//�ԐڍU���̎˒�
	static const float longAttackLength;//�������U���̎˒�

	//�ϐ�
protected:
	std::string m_name;//���O
	int m_power;//�З�
	float m_length;//�˒�
	float m_cost;//����OP
	std::shared_ptr<DamageCalculator> m_calculator;//�_���[�W�v�Z��
	std::string m_explain;//������

	//�֐�
protected:
	Weapon(const std::string &name,int power,float length,float cost,std::shared_ptr<DamageCalculator> calculator,const std::string explain):m_name(name),m_power(power),m_length(length),m_cost(cost),m_calculator(calculator),m_explain(explain){}
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
	std::string GetExplain()const{
		return m_explain;
	}
	WeaponActionKind GetActionKind()const;

	AttackInfo GetAttackInfo(const Unit *attacker,const Unit *defender)const;
	bool JudgeWeild(const Unit *attacker,const Unit *defender)const;//attacker��defender�ւ̒ʏ�U�������s�\�ł��邩
	std::string GetEffectivenessString(const Unit *attacker)const;//attacker�����̕�����g���Ƃǂ�Ȍ��ʂ𓾂��邩
	void DrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const;//�s���\�z��`�悷��

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
