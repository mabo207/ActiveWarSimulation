#ifndef DEF_DAMAGESCENE_H
#define DEF_DAMAGESCENE_H

#include"BattleSceneElement.h"
#include"ToolsLib.h"
#include"Unit.h"
#include"BattleSceneData.h"

//������HP���ς��o�g����ʂ̊��N���X
class DamageScene:public BattleSceneElement{
public:
	DamageScene(SceneKind::Kind sceneKind
		,const std::shared_ptr<BattleSceneData> &data
		,SceneKind::Kind nextSceneKind
		,Unit *aimedUnit
		,const Weapon::AttackInfo &attackInfo);
	virtual ~DamageScene();

protected:
	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();
	void thisDraw()const;
	//�p����ňقȂ�
	virtual void AnimationUpdate();
	virtual void DrawUnit()const;//�_���[�W���e�ɂ���ă��j�b�g�̕`����@���قȂ�
	virtual void DrawAnimation()const;//�A�j���[�V������������̂��������݂���\�������邽�߁B
	virtual bool JudgeAnimationEnd()const;
	virtual void DamageProcess();//�f�t�H���g��m_aimedUnit���_���[�W���󂯂邾��

	//�Q�[���̃f�[�^
	std::shared_ptr<BattleSceneData> m_battleSceneData;
	//���̃V�[��������
	const SceneKind::Kind m_nextSceneKind;
	//�U������郆�j�b�g�̃f�[�^
	Unit *const m_aimedUnit;
	//�o�߃t���[����
	int m_frame;
	//�ʒu�ύX�f�[�^
	PositionComplexControl m_damageMotion;//�_���[�W����
	//�U�����ɂ��Ẵf�[�^
	const Weapon::AttackInfo m_attackinfo;//�U�����
	const int m_damageFont;//�_���[�W�`��ɗp����t�H���g

	static const int motionFrame;//�A�j���[�V��������S�̎���
	static const int damageBeginFrame,damageEndFrame;//�_���[�W�������o������^�C�~���O�A�A�j���[�V�������I���^�C�~���O
};


#endif // !DEF_DAMAGESCENE_H
