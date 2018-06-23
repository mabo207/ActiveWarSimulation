#ifndef DEF_COMPUTERMOVESCENE_H
#define DEF_COMPUTERMOVESCENE_H

#include"MoveScene.h"
#include<vector>

//AI�����j�b�g�𓮂����ۂɎg���֐�
class ComputerMoveScene:public MoveScene{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	std::vector<Vector2D> m_route;//�ʂ�i�q�_

	//�֐�
protected:

	//���z�֐��̃I�[�o�[���C�h
	Vector2D CalculateInputVec()const;

public:
	ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);

	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();

};

#endif // !DEF_COMPUTERMOVESCENE_H
#pragma once
