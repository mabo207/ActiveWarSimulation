#ifndef DEF_SWITCHUNITSCENE_H
#define DEF_SWITCHUNITSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include"ToolsLib.h"

//�I�����郆�j�b�g��؂�ւ���V�[��
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

	//�t�H���g
	const int m_turnFont;

	//�֐�
protected:
	int CalculateTurn()const;
	std::shared_ptr<BattleSceneElement> GetPlayerMoveScene()const;

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
