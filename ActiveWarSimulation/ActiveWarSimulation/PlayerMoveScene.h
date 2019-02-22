#ifndef DEF_PLAYERMOVESCENE_H
#define DEF_PLAYERMOVESCENE_H

#include"MoveScene.h"
#include"input.h"
#include<functional>
#include<array>

//�v���C���[�����j�b�g�𓮂����ۂɎg���֐�
class PlayerMoveScene:public MoveScene{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
private:
	//�p���悩��֐����o�R�����ɕύX���ė~�����Ȃ��̂ŁAprivate
	const MouseButtonUI m_waitButton,m_researchButton,m_menuButton;
	bool m_mouseLeftFlag;//�v���C���[�����ʂŃ}�E�X�̍��N���b�N�����n�߂����ǂ���
protected:
	Vector2D m_mousePosJustBefore;//���O�t���[���ɂ�����}�E�X�̈ʒu(���ꂾ���͌p����ŕύX���Ȃ��Ƃ����Ȃ��̂�protected)

	//�֐�
protected:
	//�s���\���̔���Ƃ��̍s�������́A�N���X�O����A�N�Z�X���Ăق����Ȃ��̂�protected
	//virtual���A�_���A�[�b�^�C
	//����̌��ʂƁA�V�[���J�ڏ���Ԃ�
	std::pair<bool,int> AttackProcess();//�U���J�ڂ̔���Ə���
	std::pair<bool,int> SkillProcess();//�X�L���g�p�J�ڂ̔���Ə���
	std::pair<bool,int> CounterclockwiseProcess();//�_���̃L�����̕ύX(�����v���)�̔���Ə���
	std::pair<bool,int> ClockwiseProcess();//�_���̃L�����̕ύX(���v���)�̔���Ə���
	std::pair<bool,int> MouseSetAimedUnitProcess();//�}�E�X�ɂ��_���̃L�����̕ύX�̔���Ə���
	std::pair<bool,int> ItemProcess();//�A�C�e���g�p�J�ڂ̔���Ə���
	std::pair<bool,int> WaitProcess();//�ҋ@�J�ڂ̔���Ə���
	std::pair<bool,int> CancelProcess();//�ړ��L�����Z���̔���Ə���
	std::pair<bool,int> ResearchProcess();//�}�b�v���׃��[�h�J�ڂ̔���Ə���
	std::pair<bool,int> SystemMenuProcess();//�V�X�e�����j���[�J�ڂ̔���Ə���
	std::pair<bool,int> MoveProcess();//�ړ��̔���Ə���
	
	//���z�֐��̃I�[�o�[���C�h
	Vector2D CalculateInputVec()const;

public:
	PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	virtual ~PlayerMoveScene(){}

	//���z�֐��̃I�[�o�[���C�h
	virtual int thisCalculate();
	virtual void thisDraw()const;
	virtual void ReturnProcess();

	//�ÓI�����o
protected:
	static const std::array<std::function<std::pair<bool,int>(PlayerMoveScene&)>,11> inCalculateProcessFunction;//�������ȒP�ɂ��邽�߂ɍs���\���̔���Ƃ��̍s�������ւ̃|�C���^��ێ�

};

#endif // !DEF_PLAYERMOVESCENE_H
#pragma once
