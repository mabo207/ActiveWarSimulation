#ifndef DEF_BATTLESCENEDATA_H
#define DEF_BATTLESCENEDATA_H

#include<vector>
#include<set>
#include"Unit.h"
#include"Terrain.h"
#include"ToolsLib.h"
#include"ScoreObserver.h"
#include"LatticeBattleField.h"

//�o�g����ʂŁA�e�N���X�ɓn���Q�[���S�̂ň����f�[�^���ꊇ�Ǘ�����N���X
struct BattleSceneData{
	//�^�E�񋓑�
public:
	//���ǂ̃��[�h�ł̃v���C�����Ă��邩
	enum class PlayMode{
		e_normal//�ʏ�v���C
		,e_tutorial//�`���[�g���A�����[�h
	};

	//�萔
protected:

public:
	static const Vector2D mapDrawSize;//�}�b�v�`�敔���̑傫��
	static const Vector2D uiDrawSize;//UI�`�敔���̑傫��

	//�ϐ�
public:
	//�Q�[���Ɏg�p����ϐ�
	std::vector<BattleObject *> m_field;//�t�B�[���h�ɑ��݂���I�u�W�F�N�g�ꗗ(���I�|�C���^�̔z��)
	Unit *m_operateUnit;//���ݑ���ł��郆�j�b�g(�ÓI�|�C���^)
	std::vector<Unit *> m_unitList;//�t�B�[���h��ɐ������Ă��郆�j�b�g�ꗗ�BOP�̏��Ƀ\�[�g����Ă���B�v�f�͑S��m_field�Ɋi�[����Ă���A�擪��m_operateUnit�ɂȂ�B1���j�b�g�̍s���I���̃^�C�~���O�Ń\�[�g����B

	//�]���Ɏg�p����ϐ�
	float m_totalOP;//���܂łɏ���ꂽOP�̍��v�l�B�^�[�������v������̂Ɏg���B
	std::shared_ptr<ScoreObserver> m_scoreObserver;

	//�`��ɕK�v�ȕϐ�
	std::shared_ptr<Terrain> m_mapRange;//�}�b�v�S�̂�\������(�Ίp��)
	Vector2D m_stageSize;//�X�e�[�W�̑傫��(�Ȃ��A�X�e�[�W�ň�ԍ���ɂ���_��(0,0)�Ƃ���)

	//�ǂݍ��݂̏��
	const std::string m_stageName;

	//�O���t�B�b�N�f�[�^
	const int m_mapPic;//�}�b�v�S�̂̃O���t�B�b�N
	const int m_turnTimerPic;//�^�C�}�[�̊G
	static const size_t drawOrderHelpNum=2;
	int m_drawOrderHelp[drawOrderHelpNum];//�I�[�_�[�̖�󕪊�ɂ��āA���ꂼ��ǂ�ȈӖ�����`�悷��

	//�T�E���h�f�[�^
	//bgm
	const int m_mapBGM;//�}�b�vBGM
	//effect
	const int m_aimchangeSound;//�_�����ς�������̉�
	const int m_attackSound;//�_���[�W�̉�
	const int m_healSound;//�񕜂̉�
	const int m_footSound;//����

	//���̑��̕ϐ�
	FpsMeasuring m_fpsMesuring;//fps�v����B�^�C�}�[�̈Ӗ����������˂�B
	const int m_orderFont;//�I�[�_�[�\���̍ۂ̃t�H���g
	const PlayMode m_playMode;//���̃v���C���[�h
	

	//�f�o�b�O�p�ϐ�
public:
	bool m_drawObjectShapeFlag;//�t�B�[���h�I�u�W�F�N�g�̓����蔻��}�`��`�悷�邩�ǂ���

	//�֐�
protected:
	BattleSceneData(const std::string &stagename,const PlayMode playMode);//�p���N���X�p�R���X�g���N�^
	float CalculateOperateUnitFinishOP()const;//m_operateUnit���s���I�������ہAop�͂�����ɂȂ邩���v�Z����֐�(�s���I�����Ă��擪���j�b�g�ł����2�ԖڂɂȂ�܂�OP���������K�v�����邽��)
	float CalculateOperateUnitFinishOP(float op)const;//OP�̏���𓥂܂����v�Z���ł���悤�ɂ��邽�߂ɁA��������v�Z����֐���p�ӂ���

public:
	BattleSceneData(const std::string &stagename);
	virtual ~BattleSceneData();
	void UpdateFix();//m_field��Fix::Kind���X�V����֐�
	bool PositionUpdate(const Vector2D inputVec);//���j�b�g�̈ʒu���X�V�Am_operateUnit�Ɉړ����삪������true��Ԃ��B
	void SortUnitList();//m_unitList���\�[�g���Ȃ����B
	void FinishUnitOperation();//���̃��j�b�g�ւ̑J�ڏ���
	Unit *GetUnitPointer(Vector2D pos)const;//pos(�}�b�v��̍��W)�ɂ��郆�j�b�g��Ԃ��B���̃��j�b�g�ɍU������\�������鎖���l������const�͂��Ȃ��B
	bool CanOperateUnitMove()const;//m_operateUnit���ړ����邱�Ƃ��\���i����ɉ������邩�͍l���Ȃ��j
	int CalculateTurn()const;
	std::shared_ptr<LatticeBattleField> CalculateLatticeBattleField()const;//���݂̃X�e�[�W�̏�Ԃ̊i�q�_�F�������v�Z���ĕԂ��B

	//���`��֐�
	void DrawField(const std::set<const BattleObject *> &notDraw={})const;//�t�B�[���h�̕`��A���j�b�g�̕`��͕ʁB�������O�ɕ`�悵�����̂̓}�b�v�G�őS�ď�����͂��B
	void DrawUnit(bool infoDrawFlag,const std::set<const Unit *> &notDraw={})const;//���j�b�g�̕`��A���\��UI��\�����邩��ݒ�ł���
	void DrawHPGage()const;//�S���j�b�g��HP�Q�[�W�̕`��
	void DrawOrder(const std::set<const BattleObject *> &lineDraw={})const;//���j�b�g�̃I�[�_�[���Ԃ̕`��BlineDraw�Ɋ܂܂�郆�j�b�g�́A�K���}�b�v��̈ʒu�ƃI�[�_�[�����Ō��΂��

	//�ÓI�֐�
public:
	static bool JudgeMousePushInsideMapDrawZone(int mouseCode,bool continuousFlag);//�}�b�v�`��̈��mouseCode�����������ǂ����B�܂��AcontinuousFlag��true�ɂ���΃t���[������>0�ł��邩�ǂ������Afalse�ɂ���΃t���[������==1�ł��邩�𔻒肷��B

};


#endif // !DEF_BATTLESCENEDATA_H

#pragma once
