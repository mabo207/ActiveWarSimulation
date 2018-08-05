#ifndef DEF_PLAYERMOVESCENE_H
#define DEF_PLAYERMOVESCENE_H

#include"MoveScene.h"
#include"input.h"

//�v���C���[�����j�b�g�𓮂����ۂɎg���֐�
class PlayerMoveScene:public MoveScene{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	const MouseButtonUI m_waitButton,m_researchButton;

	//�֐�
protected:

	//���z�֐��̃I�[�o�[���C�h
	Vector2D CalculateInputVec()const;

public:
	PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	
	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();
	void thisDraw()const;
};

#endif // !DEF_PLAYERMOVESCENE_H
#pragma once
