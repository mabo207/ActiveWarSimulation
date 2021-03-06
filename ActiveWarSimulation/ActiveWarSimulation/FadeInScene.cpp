#include"FadeInScene.h"
#include"DxLib.h"
#include"CommonConstParameter.h"

//----------------FadeInScene-------------------
FadeInScene::FadeInScene(const std::shared_ptr<GameScene::SceneFactory> &nextFactory,int maxFrame)
	:m_nextScene(nextFactory?nextFactory->CreateIncompleteScene():std::shared_ptr<GameScene>())
	,m_drawAlpha(255,0,maxFrame,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0)
	,m_afterAlphaEndFrame(0)
{}

FadeInScene::~FadeInScene(){}

void FadeInScene::InitCompletely(){
	//コンストラクタ時点ではm_nextSceneは不完全な段階
	m_nextScene->InitCompletely();
}

void FadeInScene::Activate(){
	//m_nextSceneの実体ができているのでActivate()する
	m_nextScene->Activate();
}

int FadeInScene::Calculate(){
	// フェードイン処理
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

void FadeInScene::Draw()const{
	//背景色の取得
	unsigned int backColor;
	{
		int red,green,blue;
		GetBackgroundColor(&red,&green,&blue);
		backColor=GetColor(red,green,blue);
	}
	//場面の描画
	if(m_nextScene){
		//m_nextSceneが存在しているかの判定はしておく
		m_nextScene->Draw();
	}
	//透明度の表現(上に背景色長方形を被せる)
	if(m_drawAlpha.GetX()>0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,m_drawAlpha.GetX());
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,backColor,TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}
}

std::shared_ptr<GameScene> FadeInScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//そのまま返す
	return m_nextScene;
}
