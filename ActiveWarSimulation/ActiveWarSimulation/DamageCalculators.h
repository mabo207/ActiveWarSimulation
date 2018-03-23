#ifndef DEF_DAMAGECALCULATORS_H
#define DEF_DAMAGECALCULATORS_H

#include"Unit.h"
class Unit;//�z�Q�Ƃ�h�����߂ɐ錾�݂̂���

//�_���[�W�̌v�Z�����
//���N���X
class DamageCalculator{
	//�֐�
protected:
	DamageCalculator(){}
	virtual ~DamageCalculator(){}
public:
	virtual int VCalculateDamage(const Unit *attacker,const Unit *defender)const=0;//�_���[�W���v�Z����֐�
};

//�����U��
class PhysicalCalculator:public DamageCalculator{
	//�ϐ�
protected:
	double m_powerRate;//���j�b�g�́u�́v�ɂ�����{��
	double m_defRate;//���j�b�g�́u����v�ɂ�����{��
	double m_weaponRate;//����́u�З́v�ɂ�����{��

	//�֐�
public:
	PhysicalCalculator(double powerRate=1.0,double defRate=1.0,double weaponRate=1.0);
	~PhysicalCalculator();
	int VCalculateDamage(const Unit *attacker,const Unit *defender)const;
};

//���@�U��
class MagicCalculator:public DamageCalculator{
	//�ϐ�
protected:
	double m_powerRate;//���j�b�g�́u���́v�ɂ�����{��
	double m_defRate;//���j�b�g�́u���h�v�ɂ�����{��
	double m_weaponRate;//����́u�З́v�ɂ�����{��

	//�֐�
public:
	MagicCalculator(double powerRate=1.0,double defRate=1.0,double weaponRate=1.0);
	~MagicCalculator();
	int VCalculateDamage(const Unit *attacker,const Unit *defender)const;
};

#endif // !DEF_DAMAGECALCULATORS_H
#pragma once