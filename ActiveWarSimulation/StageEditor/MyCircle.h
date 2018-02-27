#ifndef DEF_MYCIRCLE_H
#define DEF_MYCIRCLE_H

#include"MyShape.h"
#include "ToolsLib.h"

//円を定義するクラス
class MyCircle :public MyShape {
	//列挙体・型
	//定数
	//変数
protected:
	
public:
	float r;
	//関数
public:
	MyCircle(float i_r);
	~MyCircle();
	void VDraw(float x, float y, unsigned int color, int fillFlag, int textureHandle)const;
	std::shared_ptr<MyShape> VCopy()const;//内容が同じでポインタの位置のみが異なるオブジェクトのポインタを返す
	void Resize(Vector2D v);//円の半径のベクトルを用いて大きさを変更する
	Vector2D GetRetResize()const;//Resizeの逆関数。引数rでResizeすると現在の図形になるようなrを返す。

	//図形内に点があるかどうかの判定
	bool VJudgePointInsideMyShape(const Vector2D point,const Vector2D shapePos)const;
	//当たり判定関数
	MyShape::HitInfo VHitJudge(const MyCircle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const;
	MyShape::HitInfo VHitJudge(const MyRectangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const;
	MyShape::HitInfo VHitJudge(const MyAngledTriangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const;
	float getTop(Vector2D aPos)const;
	float getBottom(Vector2D aPos)const;
	float getLeft(Vector2D aPos)const;
	float getRight(Vector2D aPos)const;

};

#endif // !DEF_MYCIRCLE_H
