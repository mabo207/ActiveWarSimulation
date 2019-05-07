#ifndef DEF_DAMAGECALCULATORS_H
#define DEF_DAMAGECALCULATORS_H

#include<string>

class Unit;//�z�Q�Ƃ�h�����߂ɐ錾�݂̂���

//�_���[�W�̌v�Z�����
//���N���X
class DamageCalculator{
	//�񋓑�
public:
	enum class Kind{
		e_physicalAttack
		,e_magicAttack
		,e_recover
	};

	//�ϐ�
private:
	Kind m_kind;
	
	//�֐�
protected:
	DamageCalculator(Kind kind):m_kind(kind){}
	virtual ~DamageCalculator(){}
public:
	Kind GetKind()const{
		return m_kind;
	}
	virtual int VCalculateDamage(const Unit *attacker,const Unit *defender)const=0;//�_���[�W���v�Z����֐�
	virtual bool VJudgeWeild(const Unit *attacker,const Unit *defender)const=0;//attacker��defender�ւ̒ʏ�U�������s�\�ł��邩
	virtual std::string VGetPowerString(const Unit *attacker)const=0;//attacker�����̍s�����������̌��ʂ̐��������o�͂���
	virtual void VDrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const;//�s���\�z��`�悷��B�P���ȃ_���[�W�ł���ꍇ�̕`�揈���͂����ɋL���B
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
	bool VJudgeWeild(const Unit *attacker,const Unit *defender)const;//attacker��defender�ւ̒ʏ�U�������s�\�ł��邩
	std::string VGetPowerString(const Unit *attacker)const;//attacker�����̍s�����������̌��ʂ̐��������o�͂���
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
	bool VJudgeWeild(const Unit *attacker,const Unit *defender)const;//attacker��defender�ւ̒ʏ�U�������s�\�ł��邩
	std::string VGetPowerString(const Unit *attacker)const;//attacker�����̍s�����������̌��ʂ̐��������o�͂���
};

//�񕜖��@
class RecoverCalculator:public DamageCalculator{
	//�ϐ�
protected:
	double m_powerRate;//���j�b�g�́u���́v�ɂ�����{��
	double m_weaponRate;//����́u�З́v�ɂ�����{��

	//�֐�
public:
	RecoverCalculator(double powerRate=1.0,double weaponRate=1.0);
	~RecoverCalculator();
	int VCalculateDamage(const Unit *healer,const Unit *receiver)const;
	bool VJudgeWeild(const Unit *healer,const Unit *defender)const;//attacker��defender�ւ̒ʏ�U�������s�\�ł��邩
	std::string VGetPowerString(const Unit *healer)const;//attacker�����̍s�����������̌��ʂ̐��������o�͂���
	void VDrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const;//�s���\�z��`�悷��B�񕜂Ȃ̂ŁA�U���Ƃ͈قȂ�`�������B
};

#endif // !DEF_DAMAGECALCULATORS_H
#pragma once
