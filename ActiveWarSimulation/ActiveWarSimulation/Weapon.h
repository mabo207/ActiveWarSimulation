#ifndef DEF_WEAPON_H
#define DEF_WEAPON_H

#include<string>
#include<map>
#include<memory>
#include<vector>
#include"DamageCalculators.h"
class Unit;//�z�Q�Ƃ�h�����߂ɐ錾�݂̂���

//����ɂ��ẴN���X
class Weapon{
	//�񋓑́E�^
public:
	struct AttackInfo{
		//�U�����ʂ��i�[����\����
		int damage;//�_���[�W
		//��Ԉُ�Ȃǂ�����΂�����ǉ����Ă���
		explicit AttackInfo(int i_damage=0)
			:damage(i_damage)
		{}
		~AttackInfo()=default;
	};
	enum class Kind{
		e_sword
		,e_lance
		,e_bow
		,e_book
		,e_rod
	};

	//�萔
public:
	static const float closeAttackLength;//�ߐڍU���̎˒�
	static const float openAttackLength;//�ԐڍU���̎˒�
	static const float longAttackLength;//�������U���̎˒�

	//�ϐ�
protected:
	std::string m_name;//���O
	std::string m_resisterName;//�o�^��
	Kind m_kind;//����̎��
	int m_power;//�З�
	float m_length;//�˒�
	float m_cost;//����OP
	std::shared_ptr<DamageCalculator> m_calculator;//�_���[�W�v�Z��
	std::string m_explain;//������
	AttackInfo m_turnStartDamage;//�^�[���J�n���̃_���[�W����

	//�֐�
protected:
	Weapon(const std::string &name
		,const std::string &resisterName
		,Kind kind
		,int power
		,float length
		,float cost
		,std::shared_ptr<DamageCalculator> calculator
		,const std::string &explain
		,const AttackInfo &turnStartDamage
	)
		:m_name(name)
		,m_resisterName(resisterName)
		,m_kind(kind)
		,m_power(power)
		,m_length(length)
		,m_cost(cost)
		,m_calculator(calculator)
		,m_explain(explain)
		,m_turnStartDamage(turnStartDamage)
	{}
public:
	~Weapon(){}

	std::string GetName()const{
		return m_name;
	}
	std::string GetResisterName()const{
		return m_resisterName;
	}
	Kind GetKind()const{
		return m_kind;
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
	AttackInfo GetTurnStartDamage()const{
		return m_turnStartDamage;
	}
	DamageCalculator::Kind GetActionKind()const;

	AttackInfo GetAttackInfo(const Unit *attacker,const Unit *defender)const;
	bool JudgeWeild(const Unit *attacker,const Unit *defender)const;//attacker��defender�ւ̒ʏ�U�������s�\�ł��邩
	std::string GetEffectivenessString(const Unit *attacker)const;//attacker�����̕�����g���Ƃǂ�Ȍ��ʂ𓾂��邩
	void DrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const;//�s���\�z��`�悷��

	//�ÓI�ϐ�
private:
	static const std::map<std::string,std::shared_ptr<Weapon>> s_weaponMap;//����̈ꗗ���i�[����ÓI�ϐ�
	
	//�ÓI�֐�
private:
	static std::map<std::string,std::shared_ptr<Weapon>> InitWeaponMap();
	static std::pair<std::string,std::shared_ptr<Weapon>> CreateWeaponMapElement(const std::shared_ptr<Weapon> &weapon);
public:
	static const std::shared_ptr<Weapon> GetWeapon(const std::string &name);
	static std::vector<std::shared_ptr<Weapon>> GetKindVecSorted(const Kind weaponKind);//����̕������u�R�X�g���˒����З́v�Ń\�[�g���ĕԂ�
};

#endif // !DEF_WEAPON_H
#pragma once
