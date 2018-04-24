#ifndef DEF_SWITCHUNITSCENE_H
#define DEF_SWITCHUNITSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include"ToolsLib.h"

//�I�����郆�j�b�g��؂�ւ���V�[��
class SwitchUnitScene:public BattleSceneElement{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	//�Q�[���̐i�s�f�[�^
	std::shared_ptr<BattleSceneData> m_battleSceneData;

	//�^�C�}�[


	//�֐�
protected:

	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void thisDraw()const;
	int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	void ReturnProcess();//���̏�ʂɖ߂��Ă������̏���

public:
	SwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~SwitchUnitScene();
};


#endif // !DEF_SWITCHUNITSCENE_H
#pragma once
