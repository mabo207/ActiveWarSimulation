#ifndef DEF_ATTACKSCENE_H
#define DEF_ATTACKSCENE_H

#include"DamageScene.h"
#include<memory>

//���j�b�g�𓮂����Q�[�����
class AttackScene:public DamageScene{
	//�^�E�񋓑�
public:


	//�萔
protected:
	static const float moveLength;//�A�j���[�V�������Ɉړ����鋗��

	//�ϐ�
protected:
	//�ʒu�ύX�f�[�^
	PositionComplexControl m_attackMotion;//�U�����j�b�g�̃A�j���[�V����

	//�G�t�F�N�g
	int m_effect[30];

	//�֐�
protected:
	//���z�֐��̃I�[�o�[���C�h
	void AnimationUpdate();//�U�����j�b�g�̃A�j���[�V����������ǉ�
	void DrawUnit()const;//�U�����j�b�g���A�j���[�V�������ĕ\��
	void DrawAnimation()const;//�G�t�F�N�g��\������K�v������
	bool JudgeAnimationEnd()const;//�U�����j�b�g�̃A�j���[�V�����������I������ɉ�����
	void DamageProcess();//m_operatedUnit��OP�팸���s���Ă���_���[�W����������
	int UpdateNextScene(int index);
	void ReturnProcess();

public:
	AttackScene(const std::shared_ptr<BattleSceneData> &data,Unit *aimedUnit);
	~AttackScene();
};

#endif // !DEF_ATTACKSCENE_H


#pragma once
