#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include<vector>
#include"GameScene.h"
#include"Unit.h"
#include"Terrain.h"
#include"ToolsLib.h"

//�o�g����ʕ\���N���X
class BattleScene:public GameScene{
	//�^�E�񋓑�
public:

	//�萔
protected:

public:

	//�ϐ�
protected:
	//�Q�[���̐i�s���Ǘ�����ϐ�


	//�Q�[���Ɏg�p����ϐ�
	std::vector<BattleObject *> m_field;//�t�B�[���h�ɑ��݂���I�u�W�F�N�g�ꗗ(���I�|�C���^�̔z��)
	Unit *m_operateUnit;//���ݑ���ł��郆�j�b�g(�ÓI�|�C���^)
	std::vector<Unit *> m_unitList;//�t�B�[���h��ɐ������Ă��郆�j�b�g�ꗗ�BOP�̏��Ƀ\�[�g����Ă���B�v�f�͑S��m_field�Ɋi�[����Ă���A�擪��m_operateUnit�ɂȂ�B1���j�b�g�̍s���I���̃^�C�~���O�Ń\�[�g����B
	Unit *m_aimedUnit;//���݂̍U���Ώۃ��j�b�g(�ÓI�|�C���^)

	//�O���t�B�b�N�֘A�̕ϐ�
	std::shared_ptr<Terrain> m_Window;//�E�C���h�E�S�̂�\������(�Ίp��)

	//���̑��̕ϐ�
	FpsMeasuring m_fpsMesuring;//fps�v����B�^�C�}�[�̈Ӗ����������˂�B

	//�֐�
protected:
	void UpdateFix();
	Vector2D CalculateInputVec()const;//�����^�[�����͓��͂���Ă�A�i���O�X�e�B�b�N�̃x�N�g����Ԃ��B�G�^�[������AI�ɏ]���Ĉړ������̃x�N�g����Ԃ��B
	bool PositionUpdate(const Vector2D inputVec);//���j�b�g�̈ʒu���X�V�Am_operateUnit�Ɉړ����삪������true��Ԃ��B
	void SortUnitList();//m_unitList���\�[�g���Ȃ����B
	void FinishUnitOperation();//���̃��j�b�g�ւ̑J�ڏ���
	void SetAimedUnit(float angle=0.0f,int turntimes=1);//m_aimedUnit��ύX����Bturntimes�������v���ɉ�]������
	void ProcessAttack();//�U���������s��

public:
	BattleScene(const char *stagename);
	~BattleScene();
	int Calculate();
	void Draw()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
