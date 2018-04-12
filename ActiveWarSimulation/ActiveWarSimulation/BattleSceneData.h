#ifndef DEF_BATTLESCENEDATA_H
#define DEF_BATTLESCENEDATA_H

#include<vector>
#include<set>
#include"Unit.h"
#include"Terrain.h"
#include"ToolsLib.h"

//�o�g����ʂŁA�e�N���X�ɓn���Q�[���S�̂ň����f�[�^���ꊇ�Ǘ�����N���X
struct BattleSceneData{
	//�^�E�񋓑�
public:

	//�萔
protected:

public:

	//�ϐ�
public:
	//�Q�[���Ɏg�p����ϐ�
	std::vector<BattleObject *> m_field;//�t�B�[���h�ɑ��݂���I�u�W�F�N�g�ꗗ(���I�|�C���^�̔z��)
	Unit *m_operateUnit;//���ݑ���ł��郆�j�b�g(�ÓI�|�C���^)
	std::vector<Unit *> m_unitList;//�t�B�[���h��ɐ������Ă��郆�j�b�g�ꗗ�BOP�̏��Ƀ\�[�g����Ă���B�v�f�͑S��m_field�Ɋi�[����Ă���A�擪��m_operateUnit�ɂȂ�B1���j�b�g�̍s���I���̃^�C�~���O�Ń\�[�g����B

	//�O���t�B�b�N�֘A�̕ϐ�
	std::shared_ptr<Terrain> m_Window;//�E�C���h�E�S�̂�\������(�Ίp��)

	//���̑��̕ϐ�
	FpsMeasuring m_fpsMesuring;//fps�v����B�^�C�}�[�̈Ӗ����������˂�B

	
	//�֐�
public:
	BattleSceneData(const char *stagename);
	~BattleSceneData();
	void UpdateFix();//m_field��Fix::Kind���X�V����֐�
	bool PositionUpdate(const Vector2D inputVec);//���j�b�g�̈ʒu���X�V�Am_operateUnit�Ɉړ����삪������true��Ԃ��B
	void SortUnitList();//m_unitList���\�[�g���Ȃ����B
	void FinishUnitOperation();//���̃��j�b�g�ւ̑J�ڏ���

	//���`��֐�
	void DrawField(const std::set<const BattleObject *> &notDraw={})const;//�t�B�[���h�̕`��A���j�b�g�̕`��͕�
	void DrawUnit(bool infoDrawFlag,const std::set<const Unit *> &notDraw={})const;//���j�b�g�̕`��A���\��UI��\�����邩��ݒ�ł���
	void DrawHPGage()const;//�S���j�b�g��HP�Q�[�W�̕`��
	void DrawOrder()const;//���j�b�g�̃I�[�_�[���Ԃ̕`��
};


#endif // !DEF_BATTLESCENEDATA_H

#pragma once
