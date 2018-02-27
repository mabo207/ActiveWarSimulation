#ifndef DEF_CONSTPOSSET_H
#define DEF_CONSTPOSSET_H

#include"PosSetting.h"

//定数ピクセルの方眼に合わせて位置設定をするクラス
//1ピクセルにする事で完全自由な位置設定ができるようになる
class ConstPosSet:public PosSetting{
	//型・列挙体
public:
	struct ConstPosSetButton:public PosSettingButton{
		const int m_gridSetSize;//ボタンを押すことで作るConstPosSetクラスのm_gridSizeの初期化に用いる
		ConstPosSetButton(Vector2D point,Vector2D vec,int gridSetSize);
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時に行う処理
	};
	//定数

	//変数
protected:
	const int m_gridSize;//方眼のサイズ(コンストラクタにより1以上になる)

	//関数
protected:
	Vector2D CalculateClosestVertex(Vector2D point,Vector2D vec)const;//pointからvecと逆側にある最近傍の正方形の頂点の座標を返す

public:
	ConstPosSet(Vector2D buttonPos,Vector2D buttonSize,unsigned int buttonLightColor,int gridSize);
	void DrawGuide(int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY,Vector2D adjust)const;//方眼など、位置設定の参考になるものを描画する
	Vector2D CalculatePos(Vector2D point,const EditActionSettings &settings)const;//調整した結果の位置を返す。pointはゲーム上の座標となる(=adjustによる位置修正まで終了している)。
};

#endif // !DEF_CONSTPOSSET_H
#pragma once
