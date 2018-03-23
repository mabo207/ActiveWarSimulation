#ifndef DEF_POSSETTING_H
#define DEF_POSSETTING_H

#include"ButtonHaving.h"

class EditActionSettings;

//位置合わせ設定情報を持つクラス群の基底クラス
class PosSetting:public ButtonHaving{
	//型・クラス
public:
	struct PosSettingButton:public Button{
		//変数
		Vector2D m_point,m_vec;//位置と大きさ
		std::string m_str;//表示文字列
		//関数
		PosSettingButton(Vector2D point,Vector2D vec,const std::string &str);
		void ButtonDraw(int font,int fillFlag)const;//ボタン自体を描画する
		bool JudgeInButton(Vector2D point)const;//ボタンの中にあるかの判定
		bool JudgeButtonPushed()const;//ボタンを押す動作がされたかを判定する
		//virtual void PushedProcess(EditActionSettings &settings)const=0;//ボタンが押された時に行う処理(継承先で実装)
	};

	//定数
protected:

	//変数
protected:
	//ボタンのライトアップのための情報
	Vector2D m_buttonPos,m_buttonSize;//ボタンの位置と大きさ
	unsigned int m_buttonLightColor;//ライトアップの色

	//関数
protected:

public:
	PosSetting(Vector2D buttonPos,Vector2D buttonSize,unsigned int buttonLightColor);
	void DrawPushedButtonLight()const;//該当するボタンが押されているように表示する関数
	virtual void DrawGuide(int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY,Vector2D adjust)const=0;//方眼など、位置設定の参考になるものを描画する
	virtual Vector2D CalculatePos(Vector2D point,const EditActionSettings &settings)const=0;//調整した結果の位置を返す
};

#endif // !DEF_POSSETTING_H
#pragma once
