#include<iostream>
#include"DxLib.h"
#include"GameScene.h"

//---------------------FadeInOutGameScene---------------------
FadeInOutGameScene::FadeInOutGameScene(GameScene *pActivateClass,int fadeFlag,int flame)
	:GameScene(),m_fadeFlag(fadeFlag),m_nowProcess(0),m_retIndex(0),m_pActivateClass(pActivateClass),m_timer(0)
{
	//フェードインの描画透明度の設定先を決める
	if((m_fadeFlag & FADEIN)!=0){
		//フェードインする場合
		m_drawAlpha=Easing(255,flame,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
		m_drawAlpha.SetTarget(0,true);
	} else{
		m_drawAlpha=Easing(0,flame,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
	}
}

FadeInOutGameScene::~FadeInOutGameScene(){
	//m_pActivateClassは動的確保されているはずなので解放する
	if(m_pActivateClass!=NULL){
		delete m_pActivateClass;
		m_pActivateClass=NULL;
	}
}

int FadeInOutGameScene::Calculate(){
	//透明度の更新
	m_drawAlpha.Update();
	//フェードアウト終了時の脱出
	if(m_nowProcess==1 && m_drawAlpha.GetEndFlag()){
		m_nowProcess=2;
	} else if(m_nowProcess==2){
		m_timer++;
		if(m_timer>2){
			return m_retIndex;
		}
	}
	//場面クラスのCalculate()
	int index;
	if(m_nowProcess==0){
		//m_nowProcess==0の時のみCalculate()する
		index=m_pActivateClass->Calculate();
	} else{
		index=0;
	}
	//透明度設定情報の更新(フェードアウトの検出)
	if(index!=0){
		//前場面に戻る指示が出た時
		m_retIndex=index;
		if((m_fadeFlag & FADEOUT)!=0){
			//フェードアウトを行う設定をしている場合
			m_nowProcess=1;//これ以上場面クラスのCalculate()を行わない
			m_drawAlpha=Easing(m_drawAlpha.GetX(),m_drawAlpha.GetMaxFlame()
				,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
			m_drawAlpha.SetTarget(255,true);
		} else{
			//フェードアウトを行わない設定をしている場合はそのままreturn
			return index;
		}
	}
	return 0;
}

void FadeInOutGameScene::Draw(){
	//背景色の取得
	unsigned int backColor;
	{
		int red,green,blue;
		GetBackgroundColor(&red,&green,&blue);
		backColor=GetColor(red,green,blue);
	}
	//場面の描画
	m_pActivateClass->Draw();
	//透明度の表現(上に背景色長方形を被せる)
	if(m_drawAlpha.GetX()>0){
		int dx,dy;
		GetWindowSize(&dx,&dy);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,m_drawAlpha.GetX());
		DrawBox(0,0,dx,dy,backColor,TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}
}
