#ifndef DEF_BUTTONHAVING_H
#define DEF_BUTTONHAVING_H

#include"ToolsLib.h"

class EditActionSettings;

//このクラスを継承している場合は対応するボタンが必ず存在する
//ボタンを押すと対応するクラスがEditActionSettingsの中に入るようになる

class ButtonHaving{
	//型・列挙体
public:
	struct Button{
		//Factoryクラス兼描画クラス
		virtual void ButtonDraw(int font,int fillFlag)const=0;//ボタン自体を描画する
		virtual bool JudgeInButton(Vector2D point)const=0;//ボタンの中にあるかの判定
		virtual bool JudgeButtonPushed()const=0;//ボタンを押す動作がされたかを判定する
		virtual void PushedProcess(EditActionSettings &settings)const=0;//ボタンが押された時に行う処理
	};

	//定数

	//変数

	//関数

};

#endif // !DEF_BUTTONHAVING_H
#pragma once
