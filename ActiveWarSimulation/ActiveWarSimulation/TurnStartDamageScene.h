#ifndef DEF_TURNSTARTDAMAGESCENE_H
#define DEF_TURNSTARTDAMAGESCENE_H

#include"DamageScene.h"

//�ړ��̎n�߂Ƀ_���[�W����炤�E�񕜂�������
class TurnStartDamageScene:public DamageScene{
public:
	TurnStartDamageScene(const std::shared_ptr<BattleSceneData> &data);
	~TurnStartDamageScene();
	
	static bool JudgeTurnStartDamage(const std::shared_ptr<BattleSceneData> &data);//�^�[���̎n�߂Ƀ_���[�W�������s�������łȂ��Ȃ�A�X�L�b�v����MoveScene�ɍs������
	static Weapon::AttackInfo GetAttackInfo(const std::shared_ptr<BattleSceneData> &data);

protected:
	void DrawAnimation()const;//�G�t�F�N�g��\������K�v������
	void DamageProcess();//m_aimedUnit�����񂾏ꍇ�A�J�ڐ��ς���K�v������
	int UpdateNextScene(int index);
	void ReturnProcess();

	int m_effect[30];
};

#endif // !DEF_TURNSTARTDAMAGESCENE_H
