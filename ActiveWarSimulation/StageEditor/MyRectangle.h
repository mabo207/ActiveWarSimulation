#ifndef DEF_MYRECTANGLE_H
#define DEF_MYRECTANGLE_H

#include"MyShape.h"

//長方形を定義するクラス
class MyRectangle:public MyShape {
	//型・列挙体

	//定数

	//変数
protected:
	float dx, dy;//大きさ

	//関数
public:
	MyRectangle(float i_dx,float i_dy);
	~MyRectangle();
	void VDraw(float x, float y, unsigned int color, int fillFlag, int textureHandle)const;//形状を描画する
	std::shared_ptr<MyShape> VCopy()const;//内容が同じでポインタの位置のみが異なるオブジェクトのポインタを返す
	void Resize(Vector2D v);//対角線のベクトルを用いて大きさを変更する
	Vector2D GetRetResize()const;//Resizeの逆関数。引数rでResizeすると現在の図形になるようなrを返す。

	//図形内に点があるかどうかの判定
	bool VJudgePointInsideMyShape(const Vector2D point,const Vector2D shapePos)const;
	//当たり判定関数
	MyShape::HitInfo VHitJudge(const MyCircle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const;
	MyShape::HitInfo VHitJudge(const MyRectangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const;
	MyShape::HitInfo VHitJudge(const MyAngledTriangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const;
	//四角形そのものの座標と点を引数に取り、最近傍の点を返す
	Vector2D GetNearestPoint(Vector2D rectanglePos , Vector2D p)const;	
	//上下左右の座標を取得(4分木のため)
	float getTop(Vector2D aPos)const;
	float getBottom(Vector2D aPos)const;
	float getLeft(Vector2D aPos)const;
	float getRight(Vector2D aPos)const;


};

#endif // !DEF_MYRECTANGLE_H
