#ifndef DEF_MYANGLEDTRIANGLE_H
#define DEF_MYANGLEDTRIANGLE_H

#include"MyShape.h"

//直角三角形を定義するクラス
//3点は(0,0)(dx,0)(0,dy)で表され、直角頂は(0,0)である。
class MyAngledTriangle :public MyShape {
	//列挙体・型
	//定数
	//変数
protected:
	float vx, vy;//直角頂からのx,y方向の正の方向ベクトルの長さ。負の値だと負の方向に伸びている事を示している。
	//関数
public:
	MyAngledTriangle(float i_vx,float i_vy);
	~MyAngledTriangle();
	void VDraw(float x, float y, unsigned int color, int fillFlag, int textureHandle)const;
	std::shared_ptr<MyShape> VCopy()const;//内容が同じでポインタの位置のみが異なるオブジェクトのポインタを返す
	void Resize(Vector2D v);//逆の直角三角形をくっつけた時の２直角項を結んだのベクトルを用いて大きさを変更する
	Vector2D GetRetResize()const;//Resizeの逆関数。引数rでResizeすると現在の図形になるようなrを返す。

	//図形内に点があるかどうかの判定
	bool VJudgePointInsideMyShape(const Vector2D point,const Vector2D shapePos)const;
	//当たり判定関数
	MyShape::HitInfo VHitJudge(const MyCircle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const;
	MyShape::HitInfo VHitJudge(const MyRectangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const;
	MyShape::HitInfo VHitJudge(const MyAngledTriangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const;
	//4分木のために上下左右の座標を取得する
	float getTop(Vector2D aPos)const;
	float getBottom(Vector2D aPos)const;
	float getLeft(Vector2D aPos)const;
	float getRight(Vector2D aPos)const;
	Vector2D GetNearestPoint(Vector2D trianglePos, Vector2D p)const;

};

#endif // !DEF_MYANGLEDTRIANGLE_H
