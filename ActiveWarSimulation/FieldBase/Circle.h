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
	std::shared_ptr<Shape> VCopy()const;//内容が同じでポインタの位置のみが異なるオブジェクトのポインタを返す
	float GetR()const{
		return m_r;
	}
	void Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
	Vector2D CalculatePushVec(const Shape *pShape)const;//pShapeとthisが重なっているか判定し、押し出すベクトルを返す。重なっていない場合はVector2D(0,0)が返される。
	bool VJudgePointInsideShape(Vector2D point)const;//図形内に点があるかどうかの判定、CalculatePushVecを用いるより高速に実装できるので関数を分ける
	Vector2D GetLeftTop()const;//左上の座標を求める
	Vector2D GetRightBottom()const;//右下の座標を求める
	//エディタ用
	void Resize(Vector2D v);//図形を表現するベクトルを用いて図形の大きさを変更する
	Vector2D GetRetResize()const;//Resizeの逆関数。引数rでResizeすると現在の図形になるようなrを返す。
	//読み込み・書き出し用関数
	void WriteOutShape(std::ofstream &ofs)const;//書き出し用関数
};

#endif // !DEF_CIRCLE_H
#pragma once
