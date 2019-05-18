#ifndef DEF_COMPUTERMOVESCENE_H
#define DEF_COMPUTERMOVESCENE_H

#include"MoveScene.h"
#include<vector>

//AI�����j�b�g�𓮂����ۂɎg���֐�
class ComputerMoveScene:public MoveScene{
	//�^�E�񋓑�
protected:
	//�U���������蔻�艟���o���ɂ���ď�肭�����Ȃ������ꍇ�̏��Ǘ�
	struct AttackFailedInfo{
		//�ϐ�
		size_t m_count;//���s������
		size_t m_frame;//�O�񎸔s����o�߂����t���[����
		//�֐�
		AttackFailedInfo();
		~AttackFailedInfo(){}
		bool JudgeAttackProcessProceed()const;//�U�������̕���ɐi�܂��đ��v��
		bool JudgeRetry()const;//�ړ�����蒼�����邩
		void RetryProcess();//��蒼������
	};

	//�萔
protected:

	//�ϐ�
protected:
	//�i�q�_�̒�`�̂��߂̏��
	const std::shared_ptr<LatticeBattleField> m_latticeField;

	//AI�̂��߂̏��
	std::vector<std::pair<size_t,Vector2D>> m_latticeRoute;//�ʂ�i�q�_(size_t:�i�q�_�ԍ�(m_latticeInShape.size()���I�[�o�[����ꍇ�͊i�q�_�ł͂Ȃ����Ƃ�����),Vector2D:���̈ʒu�B�덷�ɂ���Čv�Z���ʒu���琳�����i�q�_�ԍ������߂��Ȃ����O������̂ł��������B)
	Unit *m_targetUnit;//�_�����Ƃ��Ă��郆�j�b�g
	bool m_actionWaiting;//�s�����s���܂ł̑҂����Ԃ�҂��Ă����Ԃ��H�i�ΏەύX�����u�ԂɍU�����ƕ�����Â炻���j
	int m_nextScene;//�ǂ̍s����ʂ֐i�ނ�(thisCalculate()�̕Ԃ�l������)
	AttackFailedInfo m_attackFailedInfo;//�����o�������ɂ��U���̎��s�̃��J�o���[�̂��߂̏��

	//���̑��̏��
	size_t m_aimChangeFrame;//�_����ύX���Ă���o��������
	size_t m_waitingFrame;//�ҋ@��Ԃ̃t���[����

	std::vector<LatticeBattleField::LatticeDistanceInfo> distvec;//�f�o�b�O�p���������ϐ�

	//�֐�
protected:
	std::pair<std::pair<size_t,Vector2D>,Unit *> DecideTargetPoint(const std::vector<LatticeBattleField::LatticeDistanceInfo> &distanceInfo)const;//�ڕW�Ƃ���i�q�_�Ƃ��̊i�q�_�ɓ����������Ƃɑ_���ʒu�����߂�
	void CalculateLatticeRoute();//m_latticeInShape����m_latticeRoute���v�Z����B�s�����Čv�Z����ꍇ�ɂ��p����B
	float CalculateEvaluate(const Unit *punit,const std::vector<LatticeBattleField::LatticeDistanceInfo> &distanceInfo)const;
	float CalculateActionEvaluate(const Unit *punit)const;//�s�������ۂ̉e���x�ɂ��Ă̕]���l�ACalculateEvaluate()�̑��ɂ��A���̏�ҋ@���邩�ǂ����̔���ɗp���邱�Ƃ�����̂ŕ����B
	int BranchingWaitingProcess();//AI�s�����ɂ��̏�ҋ@���邱�ƂɂȂ������A���̏�ōU���ł���L����������Ȃ�U������悤�ɏ����𕪊򂷂邽�߂̊֐�

	//���z�֐��̃I�[�o�[���C�h
	Vector2D CalculateInputVec()const;

public:
	ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~ComputerMoveScene();

	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();
	void thisDraw()const;

};

#endif // !DEF_COMPUTERMOVESCENE_H
#pragma once
