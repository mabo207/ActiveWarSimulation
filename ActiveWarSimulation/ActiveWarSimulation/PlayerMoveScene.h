#ifndef DEF_PLAYERMOVESCENE_H
#define DEF_PLAYERMOVESCENE_H

#include"MoveScene.h"

//�v���C���[�����j�b�g�𓮂����ۂɎg���֐�
class PlayerMoveScene:public MoveScene{
	//�^�E�񋓑�

	//�萔

	//�ϐ�

	//�֐�
protected:

	//���z�֐��̃I�[�o�[���C�h
	Vector2D CalculateInputVec()const;
public:
	PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);

};

#endif // !DEF_PLAYERMOVESCENE_H
#pragma once
