#ifndef DEF_COMPUTERMOVESCENE_H
#define DEF_COMPUTERMOVESCENE_H

#include"MoveScene.h"

//AI�����j�b�g�𓮂����ۂɎg���֐�
class ComputerMoveScene:public MoveScene{
	//�^�E�񋓑�

	//�萔

	//�ϐ�

	//�֐�
protected:

	//���z�֐��̃I�[�o�[���C�h
	Vector2D CalculateInputVec()const;
public:
	ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);

};

#endif // !DEF_COMPUTERMOVESCENE_H
#pragma once
