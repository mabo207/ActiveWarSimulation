#include"DxLib.h"
#include"TutorialPlayerMoveScene.h"


//-------------------TutorialPlayerMoveScene---------------
TutorialPlayerMoveScene::TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:PlayerMoveScene(battleSceneData),m_tutorialBattleSceneData(std::dynamic_pointer_cast<TutorialBattleSceneData>(battleSceneData)){}

TutorialPlayerMoveScene::~TutorialPlayerMoveScene(){}

void TutorialPlayerMoveScene::thisDraw()const{
	PlayerMoveScene::thisDraw();

	DrawCircle(50,50,40,GetColor(255,0,0),TRUE);

}
