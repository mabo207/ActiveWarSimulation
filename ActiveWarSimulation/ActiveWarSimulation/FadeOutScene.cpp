#include"FadeOutScene.h"
#include"DxLib.h"
#include"CommonConstParameter.h"

//----------------FadeOutScene-------------------
FadeOutScene::FadeOutScene(const std::shared_ptr<GameScene> &previousScene,const std::shared_ptr<GameScene::SceneFactory> &nextFactory,int maxFrame)
	:m_drawAlpha(0,255,maxFrame,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0)
	,m_previousScene(previousScene)
	,m_nextFactory(nextFactory)
	,m_afterAlphaEndFrame(0)
{}

FadeOutScene::~FadeOutScene(){}

int FadeOutScene::Calculate(){
	// フェードアウト処理
	m_drawAlpha.Update();
	if(m_drawAlpha.GetEndFlag()){
		// フェードアウトが終了したらm_afterAlphaEndFrameを加算
		m_afterAlphaEndFrame++;
	}
	if(m_afterAlphaEndFrame>2){
		// フェードアウトが終了してから2f経ったら次の場面へ
		return 1;
	}
	return 0;
}

void FadeOutScene::Draw()const{
	//背景色の取得
	unsigned int backColor;
	{
		int red,green,blue;
		GetBackgroundColor(&red,&green,&blue);
		backColor=GetColor(red,green,blue);
	}
	//場面の描画
	if(m_previousScene){
		//m_previousSceneが存在しているかの判定はしておく
		m_previousScene->Draw();
	}
	//透明度の表現(上に背景色長方形を被せる)
	if(m_drawAlpha.GetX()>0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,m_drawAlpha.GetX());
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,backColor,TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}
}

std::shared_ptr<GameScene> FadeOutScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//m_nextFactoryを用いて作成
	if(m_nextFactory){
		//m_nextFactoryが実体を持つかどうか
		return m_nextFactory->CreateScene();
	}
	return std::shared_ptr<GameScene>();
}
