#ifndef DEF_BASEUIINSTAGESELECT_H
#define DEF_BASEUIINSTAGESELECT_H

#include<memory>
#include"StageLevel.h"
#include"ToolsLib.h"

//�o�g���}�b�v�I����ʂ�UI�̊��N���X
class BaseUIInStageSelect{
public:
	//UI���Ǘ�����f�[�^
	struct ControledData{
		size_t stageIndex;
		StageLevel selectLevel;
		PositionControl stageInfoPos;
		PositionControl levelInfoPos;
		ControledData(size_t i_stageIndex,StageLevel i_selectLevel);
		~ControledData(){}
	};

	//�X�V�����̌���
	enum class UpdateResult{
		e_notTransition		//�J�ڂ��Ȃ�
		,e_gotoTitle		//�^�C�g����ʂɑJ��
		,e_gotoBattle		//�o�g����ʂɑJ��
		,e_gotoStageSelect	//�X�e�[�W�Z���N�gUI�ɑJ��
		,e_gotoLevelSelect	//���x���I��UI�ɑJ��
	};

	virtual ~BaseUIInStageSelect();
	virtual UpdateResult Update()=0;
	virtual void Draw()const=0;//UI�̕\��

	static const int infoDrawAreaWidth;//���`��ł���E���̗̈�̉���(�f�ނɂ���Č��܂�̂ŁA�f�ނ����ē���)

protected:
	BaseUIInStageSelect(const std::weak_ptr<ControledData> &controledData);

	const std::weak_ptr<ControledData> m_controledData;
};

#endif // !DEF_BASEUIINSTAGESELECT_H