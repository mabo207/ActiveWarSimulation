#include<iostream>
#include"DxLib.h"
#include"GameScene.h"
#include"CommonConstParameter.h"


#include"FadeInScene.h"
#include"FadeOutScene.h"

//---------------------GameScene---------------------
std::shared_ptr<GameScene> GameScene::CreateFadeOutInScene(const std::shared_ptr<GameScene> &thisSharedPtr,const std::shared_ptr<GameScene::SceneFactory> &nextFactory,const int fadeOutFrame,const int fadeInFrame){
	const auto fadeInFactory=std::make_shared<FadeInScene::FadeInSceneFactory>(nextFactory,fadeInFrame);
	const auto fadeOutFactory=std::make_shared<FadeOutScene::FadeOutSceneFactory>(thisSharedPtr,fadeInFactory,fadeOutFrame);
	return fadeOutFactory->CreateScene();
}
