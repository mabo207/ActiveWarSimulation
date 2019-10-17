#ifndef DEF_SWITCHUNITSCENE_H
#define DEF_SWITCHUNITSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include"ToolsLib.h"

//�I�����郆�j�b�g��؂�ւ���V�[��
//������p������ƁAReturnProcess()�����s����܂ŃR���X�g���N�^�Ń��j�b�g�̍s�����̏�����������Ȃ�
class SwitchUnitScene:public BattleSceneElement{
	//�^�E�񋓑�
protected:
	enum class JudgeEnd{
		e_notEnd
		,e_notEnd_TurnChange
		,e_playerWin
		,e_playerLose
	};

	//�萔

	//�ϐ�
protected:
	//���s����
	JudgeEnd m_judgeEnd;

	//�Q�[���̐i�s�f�[�^
	std::shared_ptr<BattleSceneData> m_battleSceneData;

	//���[�u���b�N�]���`��
	PositionControl m_rubricWordPosition;//�]���̕`��ʒu
	PositionControl m_rubricReasonPosition;//�]�����R�̕`��ʒu
	bool m_rubricPopupExiting;//���R�̃|�b�v�A�b�v����ʊO�ɏo�鏈�����ł���

	//�t�H���g
	const int m_turnFont;

	//�֐�
protected:
	int CalculateTurn()const;
	virtual std::shared_ptr<BattleSceneElement> GetPlayerMoveScene()const;//�J�ڐ��ύX����K�v������΍X�V���Ă��ǂ�
	bool JudgeTimeProcessed()const;//�K�莞�Ԍo�������ǂ���
	bool JudgeGoToMoveScene()const;//�ړ��V�[���ɍs�����ǂ���
	bool JudgeDrawRubric()const;//���[�u���b�N�]����`�悷�邩�ǂ���

	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void thisDraw()const;
	virtual int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B�f�����Ƃ������ς��B
	void ReturnProcess();//���̏�ʂɖ߂��Ă������̏���

public:
	SwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	virtual ~SwitchUnitScene();
};


#endif // !DEF_SWITCHUNITSCENE_H
#pragma once
