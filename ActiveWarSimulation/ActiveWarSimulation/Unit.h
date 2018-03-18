#ifndef DEF_UNIT_H
#define DEF_UNIT_H

#include"BattleObject.h"

//���j�b�g������
class Unit:public BattleObject{
	//�^�E�񋓑�
public:
	struct Team{
		enum Kind{
			e_player,
			e_enemy,
			END
		};
		static Kind link(int num);
		static unsigned int GetColor(Kind kind);
	};
	struct BaseStatus{
		int lv;
		int maxHP;
		int power;
		int def;
		int mpower;
		int mdef;
		int speed;
		int move;
		BaseStatus(int i_lv,int i_maxHP,int i_power,int i_def,int i_mpower,int i_mdef,int i_speed,int i_move)
			:lv(i_lv),maxHP(i_maxHP),power(i_power),def(i_def),mpower(i_mpower),mdef(i_mdef),speed(i_speed),move(i_move){}
	};
	struct BattleStatus{
		//�萔
		static const float maxOP;//�I�[�_�[�|�C���g�̍ő�l�A�S�Ẵ��j�b�g�ňꗥ
		//�ϐ�
		int HP;
		float OP;
		Team::Kind team;
		BattleStatus(int i_HP,float i_OP,Team::Kind i_team)
			:HP(i_HP),OP(i_OP),team(i_team){}
	};

	//�萔
protected:
	static const float unitCircleSize;//���j�b�g���̂��̂̑傫��
	static const float rivalInpenetratableCircleSize;//�G�΃��j�b�g�̐i���s�̈�
	static const float closeAttackLength;//�Z�����U���̎˒�
	static const float openAttackLength;//�������U���̎˒�

	static const float attackCost;//�U���̃R�X�g������(����͕���N���X�ɓ˂����މ\��������)

public:

	//�ϐ�
protected:
	//��{�X�e�[�^�X
	BaseStatus m_baseStatus;
	//�퓬���X�e�[�^�X
	BattleStatus m_battleStatus;
	std::shared_ptr<Shape> m_rivalInpenetratableCircle;
	bool m_penetratable;//�����̃t�F�C�Y�̂ɓG�΃��j�b�g�i���s�̈�ɐN���ł��邩�ǂ���
	//�`��n�ϐ�
	//�}�b�v��̃L�����O���t�B�b�N(BattleObject�ɂ���)
	//�X�e�[�^�X��ʂ̃L�����O���t�B�b�N

public:
	//����
	//�K�E�Z
	//�A�C�e��(������)

	//�֐�
protected:
	void WriteOutObjectPeculiarInfo(std::ofstream &ofs)const;//�I�u�W�F�N�g���L�̏���Ԃ�

public:
	//�R���X�g���N�^�n
	Unit(Vector2D position,int gHandle,Team::Kind team);
	~Unit();
	//���Z�q�I�[�o�[���[�h

	//getter�n
	BaseStatus GetBaseStatus()const{
		return m_baseStatus;
	}
	BattleStatus GetBattleStatus()const{
		return m_battleStatus;
	}
	//�ʏ�֐�
	bool SetPenetratable(Team::Kind nowPhase);//���ݓ������Ă��郆�j�b�g�̏��������āA�N���ۂ̃t���O���X�V����
	bool JudgeAttackable(const Unit *pUnit)const;//pUnit�ɍU���\��
	const Shape *GetUnitCircleShape()const;//���j�b�g���g�������~���擾
	int AddHP(int pal);//HP�𑝌�������֐��A�������HP��Ԃ�
	void AddOP(float cost);//OP�𑝌�������֐�
	float CalculateAddOPNormalAttack()const;//�ʏ�U���ɂ���đ�������OP���v�Z����B���l���Ԃ�B
	void DrawMoveInfo(Vector2D adjust=Vector2D())const;//�ړ��͈͊֘A�̏���`�悷��֐�(VDraw()�Ɠ����悤�ȃI�[�o�[���[�h������)
	void DrawMoveInfo(Vector2D point,Vector2D adjust)const;//�ړ��͈͊֘A�̏���`�悷��֐�
	
	//���z�֐��E�������z�֐��̃I�[�o�[���C�h
	const Shape *GetHitJudgeShape()const;//�����蔻��}�`���擾�B
	void Move(Vector2D v);//�����蔻��}�`�̈ړ��B�Q��ނ̓����蔻��}�`�����Ɉړ�������B
	Shape::Fix::Kind SetFix(Shape::Fix::Kind fix)const;//m_hitJudgeShape.m_fix��ύX
	void VDraw(Vector2D point,Vector2D adjust)const;//�`�揈��(�C�Ӎ��W�ɕ`��)
	void VHitProcess(const BattleObject *potherobj);//�����ɓ����������̏���
	std::shared_ptr<BattleObject> VCopy()const;//�����I�u�W�F�N�g�𕡐�����(�|�C���^�݈̂قȂ�)
};

#endif // !DEF_UNIT_H

#pragma once