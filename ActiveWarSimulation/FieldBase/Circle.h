#ifndef DEF_CIRCLE_H
#define DEF_CIRCLE_H

#include"Shape.h"

//円を表すクラス
class Circle:public Shape{
	//型・列挙体

	//定数

	//変数
protected:
	float m_r;//半径

	//関数
public:
	Circle(Vector2D position,float r,Fix::Kind fix);
	~Circle();
	float GetR()const{
		return m_r;
	}
	void Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
	Vector2D CalculatePushVec(const Shape *pShape)const;//pShapeとthisが重なっているか判定し、押し出すベクトルを返す。重なっていない場合はVector2D(0,0)が返される。
	Vector2D GetLeftTop()const;//左上の座標を求める
	Vector2D GetRightBottom()const;//右下の座標を求める
};

#endif // !DEF_CIRCLE_H
#pragma once
