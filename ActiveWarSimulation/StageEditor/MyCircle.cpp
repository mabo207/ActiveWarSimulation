#include"DxLib.h"
#include"MyCircle.h"

MyCircle::MyCircle(float i_r):r(i_r) {}

MyCircle::~MyCircle() {}

void MyCircle::VDraw(float x, float y, unsigned int color, int fillFlag, int textureHandle)const{
	//ひとまずテクスチャーを使わずに実装
	DrawCircle((int)x, (int)y ,(int)r, color, fillFlag);
}

std::shared_ptr<MyShape> MyCircle::VCopy()const{
	return std::shared_ptr<MyShape>(new MyCircle(r));
}

void MyCircle::Resize(Vector2D v){
	//vの長さを半径にしてあげれば良い
	r=v.size();
}

Vector2D MyCircle::GetRetResize()const{
	//長さrのベクトルを返せばなんでもよい
	return Vector2D(r,0);
}

MyShape::HitInfo MyCircle::VHitJudge(const MyCircle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const{
	return MyShape::HitJudge(this,pshape,aMyPos,aOtherPos);
}

MyShape::HitInfo MyCircle::VHitJudge(const MyRectangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const{
	return MyShape::HitJudge(this, pshape,aMyPos,aOtherPos);
}

MyShape::HitInfo MyCircle::VHitJudge(const MyAngledTriangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const{
	return MyShape::HitJudge(this, pshape,aMyPos,aOtherPos);
}

bool MyCircle::VJudgePointInsideMyShape(const Vector2D point, const Vector2D shapePos)const{
	return ((point - shapePos).size() <= r);
}

float MyCircle::getTop(Vector2D aPos)const{
	return aPos.y - r;
}
float MyCircle::getBottom(Vector2D aPos)const{
	return aPos.y + r;
}
float MyCircle::getLeft(Vector2D aPos)const{
	return aPos.x - r;
}
float MyCircle::getRight(Vector2D aPos)const{
	return aPos.x + r;
}