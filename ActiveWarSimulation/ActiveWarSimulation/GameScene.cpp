#include<iostream>
#include"DxLib.h"
#include"GameScene.h"
#include"CommonConstParameter.h"


#include"FadeInScene.h"
#include"FadeOutScene.h"
#include"LoadingScene.h"

//---------------------GameScene---------------------
std::shared_ptr<GameScene> GameScene::CreateFadeOutInSceneCompletely(const std::shared_ptr<GameScene> &thisSharedPtr,const std::shared_ptr<GameScene::SceneFactory> &nextFactory,const int fadeOutFrame,const int fadeInFrame){
	const auto fadeInFactory=std::make_shared<FadeInScene::FadeInSceneFactory>(nextFactory,fadeInFrame);
	const auto loadingFactory=std::make_shared<LoadingScene::LoadingSceneFactory>(fadeInFactory);
	const auto fadeOutFactory=std::make_shared<FadeOutScene::FadeOutSceneFactory>(thisSharedPtr,loadingFactory,fadeOutFrame);
	return fadeOutFactory->CreateCompleteScene();
}
