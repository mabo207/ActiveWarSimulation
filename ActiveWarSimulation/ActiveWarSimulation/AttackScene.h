#ifndef DEF_ATTACKSCENE_H
#define DEF_ATTACKSCENE_H

#include"GameScene.h"
#include"BattleSceneData.h"
#include<memory>

//���j�b�g�𓮂����Q�[�����
class AttackScene:public GameScene{
	//�^�E�񋓑�
public:


	//�萔
protected:
	static const int motionFlame;//�A�j���[�V��������S�̎���
	static const int damageBeginFlame,damageEndFlame;//�_���[�W�������o������^�C�~���O�A�A�j���[�V�������I���^�C�~���O

	//�ϐ�
protected:
	//�Q�[���̃f�[�^
	std::shared_ptr<BattleSceneData> m_battleSceneData;

	//�U������郆�j�b�g�̃f�[�^
	Unit *m_aimedUnit;

	//�ʒu�ύX�f�[�^
	PositionComplexControl m_attackMotion;//�U�����j�b�g
	PositionControl m_damageMotion;//�_���[�W����

	//�U�����ɂ��Ẵf�[�^
	Weapon::AttackInfo m_attackinfo;//�U�����
	int m_damageFont;//�_���[�W�`��ɗp����t�H���g

	//�֐�
public:
	AttackScene(std::shared_ptr<BattleSceneData> data,Unit *aimedUnit);
	~AttackScene();

	//���z�֐��̃I�[�o�[���C�h
	int Calculate();
	void Draw()const;

};

#endif // !DEF_ATTACKSCENE_H


#pragma once
