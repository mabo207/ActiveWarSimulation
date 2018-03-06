#ifndef DEF_SCROLLBAR_H
#define DEF_SCROLLBAR_H

#include"ButtonHaving.h"

//スクロールバーを管理するクラス
//上下左右のスクロールバー全てに対応できるようにするため、ボタンの位置を定数にしない
class ScrollBar:public ButtonHaving{
	//型
public:
	struct ScrollButton:public Button{
		//変数
		static const int vNum=4;//頂点数
		Vector2D m_point[vNum];//スクロールバーを成す４点
		Vector2D m_move;//ボタンを押した時に動くベクトル
		
			//色・塗りつぶしの可否などは固定して、ソースコードにベタ書きする

		//関数
		ScrollButton(Vector2D p1,Vector2D p2,Vector2D p3,Vector2D p4,Vector2D v);
		void ButtonDraw(int font,int fillFlag)const;//ボタン自体を描画する
		bool JudgeInButton(Vector2D point)const;//ボタンの中にあるかの判定
		bool JudgeButtonPushed()const;//ボタンを押す動作がされたかを判定する
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時に行う処理
	};
	//定数
	
	//ScrollBarクラスは作られる事はないので変数・関数は実装しない

	//変数
protected:
	

	//関数
public:
	
};

#endif // !DEF_SCROLLBAR_H
#pragma once
