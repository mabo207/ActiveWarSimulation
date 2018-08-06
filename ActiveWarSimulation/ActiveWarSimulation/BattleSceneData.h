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

	//�`��ɕK�v�ȕϐ�
	std::shared_ptr<Terrain> m_Window;//�E�C���h�E�S�̂�\������(�Ίp��)
	Vector2D m_stageSize;//�X�e�[�W�̑傫��(�Ȃ��A�X�e�[�W�ň�ԍ���ɂ���_��(0,0)�Ƃ���)

	//�O���t�B�b�N�f�[�^
	const int m_mapPic;//�}�b�v�S�̂̃O���t�B�b�N
	
	//���̑��̕ϐ�
	FpsMeasuring m_fpsMesuring;//fps�v����B�^�C�}�[�̈Ӗ����������˂�B
	const int m_orderFont;//�I�[�_�[�\���̍ۂ̃t�H���g
	static const size_t drawOrderHelpNum=2;
	int m_drawOrderHelp[drawOrderHelpNum];//�I�[�_�[�̖�󕪊�ɂ��āA���ꂼ��ǂ�ȈӖ�����`�悷��


	//�f�o�b�O�p�ϐ�
public:
	bool m_drawObjectShapeFlag;//�t�B�[���h�I�u�W�F�N�g�̓����蔻��}�`��`�悷�邩�ǂ���

	//�֐�
protected:
	float CalculateOperateUnitFinishOP()const;//m_operateUnit���s���I�������ہAop�͂�����ɂȂ邩���v�Z����֐�(�s���I�����Ă��擪���j�b�g�ł����2�ԖڂɂȂ�܂�OP���������K�v�����邽��)
	float CalculateOperateUnitFinishOP(float op)const;//OP�̏���𓥂܂����v�Z���ł���悤�ɂ��邽�߂ɁA��������v�Z����֐���p�ӂ���

public:
	BattleSceneData(const char *stagename);
	~BattleSceneData();
	void UpdateFix();//m_field��Fix::Kind���X�V����֐�
	bool PositionUpdate(const Vector2D inputVec);//���j�b�g�̈ʒu���X�V�Am_operateUnit�Ɉړ����삪������true��Ԃ��B
	void SortUnitList();//m_unitList���\�[�g���Ȃ����B
	void FinishUnitOperation();//���̃��j�b�g�ւ̑J�ڏ���
	Unit *GetUnitPointer(Vector2D pos)const;//pos(�}�b�v��̍��W)�ɂ��郆�j�b�g��Ԃ��B���̃��j�b�g�ɍU������\�������鎖���l������const�͂��Ȃ��B

	//���`��֐�
	void DrawField(const std::set<const BattleObject *> &notDraw={})const;//�t�B�[���h�̕`��A���j�b�g�̕`��͕ʁB�������O�ɕ`�悵�����̂̓}�b�v�G�őS�ď�����͂��B
	void DrawUnit(bool infoDrawFlag,const std::set<const Unit *> &notDraw={})const;//���j�b�g�̕`��A���\��UI��\�����邩��ݒ�ł���
	void DrawHPGage()const;//�S���j�b�g��HP�Q�[�W�̕`��
	void DrawOrder(const std::set<const BattleObject *> &lineDraw={})const;//���j�b�g�̃I�[�_�[���Ԃ̕`��BlineDraw�Ɋ܂܂�郆�j�b�g�́A�K���}�b�v��̈ʒu�ƃI�[�_�[�����Ō��΂��
};


#endif // !DEF_BATTLESCENEDATA_H

#pragma once
