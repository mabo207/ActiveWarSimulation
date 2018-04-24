#include"input.h"
#include"DxLib.h"
#include"PlayerMoveScene.h"

//----------------------PlayerMoveScene------------------------
PlayerMoveScene::PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData){}

Vector2D PlayerMoveScene::CalculateInputVec()const{
	Vector2D moveVec;
	moveVec=analogjoypad_get(DX_INPUT_PAD1);
	//�A�i���O�X�e�B�b�N�̕����I�ȃY�����ɂ��moveVec�̔����͂�����
	const float gap=50.0f;
	if(std::abs(moveVec.x)<gap){
		moveVec.x=0.0f;
	}
	if(std::abs(moveVec.y)<gap){
		moveVec.y=0.0f;
	}
	return moveVec;
}


