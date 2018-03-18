#ifndef DEF_GAMESCENE_H
#define DEF_GAMESCENE_H

#include"ToolsLib.h"

//場面を表すクラスが持つべき実装のインターフェース
//かつこれだけあれば他クラスから動かすことが出来る

class GameScene{
	//型
	//定数
	//変数
	//関数
public:
	virtual int Calculate()=0;
	virtual void Draw()const=0;
	GameScene(){}
	virtual ~GameScene(){}
};

//フェードイン、フェードアウトをする機能をGameSceneに追加するクラス
//実際に動くクラスをpClassとして、pClassを動的確保してからFadeInOutGameScene(pClass,int,int)とする事で宣言できる。
class FadeInOutGameScene:public GameScene{
	//型
public:
	enum FADEFLAG{
		FADEIN=0x01,
		FADEOUT=0x02
	};
	//定数

	//変数
protected:
	const int m_fadeFlag;//フェードイン、フェードアウトするかのフラグ管理をしている
	Easing m_drawAlpha;//描画透明度をxに管理
	int m_nowProcess;//現在の段階(0:通常進行 1:Calculate()を行わずにフェードアウト 2:フェードアウト完了待ち)
	int m_retIndex;//フェードアウト終了後Calculate()が返す数
	int m_timer;//フェードアウトが綺麗に行われるために用いる
	GameScene *m_pActivateClass;//実際に動かす場面クラス

	//関数
public:
	FadeInOutGameScene(GameScene *pActivateClass,int fadeFlag,int flame);
	~FadeInOutGameScene();
	int Calculate();
	void Draw()const;
};


#endif // !DEF_GAMESCENE_H

#pragma once
