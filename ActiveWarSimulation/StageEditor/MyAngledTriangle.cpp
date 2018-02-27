#include"DxLib.h"
#include"MyAngledTriangle.h"

MyAngledTriangle::MyAngledTriangle(float i_vx, float i_vy)
	:vx(i_vx),vy(i_vy){}

MyAngledTriangle::~MyAngledTriangle() {}

void MyAngledTriangle::VDraw(float x, float y, unsigned int color, int fillFlag, int textureHandle)const {
	//ひとまずテクスチャーを使わずに実装
	DrawTriangle((int)x,(int)y
		,(int)(x+vx),(int)y
		,(int)x,(int)(y+vy)
		,color, fillFlag);
}

std::shared_ptr<MyShape> MyAngledTriangle::VCopy()const{
	return std::shared_ptr<MyShape>(new MyAngledTriangle(vx,vy));
}

void MyAngledTriangle::Resize(Vector2D v){
	vx=v.x;
	vy=v.y;
}

Vector2D MyAngledTriangle::GetRetResize()const{
	return Vector2D(vx,vy);
}

MyShape::HitInfo MyAngledTriangle::VHitJudge(const MyCircle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const{
	//ここは第二引数が自分のため、aMyPosは第四引数にしないとおかしい
	return MyShape::HitJudge(pshape, this,aOtherPos,aMyPos);
}

MyShape::HitInfo MyAngledTriangle::VHitJudge(const MyRectangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const{
	//ここは第二引数が自分のため、aMyPosは第四引数にしないとおかしい
	return MyShape::HitJudge(pshape,this,aOtherPos,aMyPos);
}

MyShape::HitInfo MyAngledTriangle::VHitJudge(const MyAngledTriangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const{
	return MyShape::HitJudge(this, pshape, aMyPos, aOtherPos);
}

bool MyAngledTriangle::VJudgePointInsideMyShape(const Vector2D point, const Vector2D shapePos)const{
	//まず長方形内部にあるかどうかの判定
	const Vector2D sp = point - shapePos;
	if (sp.x >= getLeft(Vector2D(0,0)) && sp.x <= getRight(Vector2D(0,0)) && sp.y >= getTop(Vector2D(0,0)) && sp.y <= getBottom(Vector2D(0,0))) {
		//そしてpointとshapePosが対角線に対して同じ側にあるかを判定すればよい
		//shapePosを原点とした時の対角線の方程式:y=(vy-0)/(0-vx)*(x-vx) -> y*vx+vy*(x-vx)=0
		//同じ側にあるかの判定をすればよいので、vxの正負を考える必要はない
		return ((-vy*vx)*(sp.y*vx + vy*(sp.x - vx)) >= 0);
	}
	return false;
}

float MyAngledTriangle::getTop(Vector2D aPos)const{
	return min(aPos.y,aPos.y+vy);
}
float MyAngledTriangle::getBottom(Vector2D aPos)const{
	return max(aPos.y,aPos.y+vy);
}
float MyAngledTriangle::getLeft(Vector2D aPos)const{
	return min(aPos.x,aPos.x+vx);
}
float MyAngledTriangle::getRight(Vector2D aPos)const{
	return max(aPos.x,aPos.x+vx);
}

Vector2D MyAngledTriangle::GetNearestPoint(Vector2D trianglePos, Vector2D p)const {
	bool top = false, left = false,right=false,bottom=false;
	
	if (vx > 0 && vy > 0) {
		//三角形の形状により場合分け
		if (p.x < getLeft(trianglePos)) {
			//もし点(円の中心を想定)が三角形の左端より左にあったら
			p.x = getLeft(trianglePos);
			left = true;
		}
		//yにも同様の処理をする
		if (p.y < getTop(trianglePos)) {
			p.y = getTop(trianglePos);
			top = true;
		}
		if (left || top) {
			if (p.x > getRight(trianglePos)) {
				//もし点が三角形の右端より右にあったら
				p.x = getRight(trianglePos);
			}
			if (p.y > getBottom(trianglePos)) {
				p.y = getBottom(trianglePos);
			}
			return p;
		}
	}

	if (vx > 0 && vy < 0) {
		//三角形の形状により場合分け
		if (p.x < getLeft(trianglePos)) {
			//もし点(円の中心を想定)が三角形の左端より左にあったら
			p.x = getLeft(trianglePos);
			left = true;
		}
		//yにも同様の処理をする
		if (p.y > getBottom(trianglePos)) {
			p.y = getBottom(trianglePos);
			bottom = true;
		}
		if (left || bottom) {
			if (p.x > getRight(trianglePos)) {
				//もし点が三角形の右端より右にあったら
				p.x = getRight(trianglePos);
			}
			if (p.y < getTop(trianglePos)) {
				p.y = getTop(trianglePos);
			}
			return p;
		}
	}
	if (vx < 0 && vy > 0) {
		//三角形の形状により場合分け
		if (p.x > getRight(trianglePos)) {
			//もし点(円の中心を想定)が三角形の右端より右にあったら
			p.x = getRight(trianglePos);
			right = true;
		}
		//yにも同様の処理をする
		if (p.y < getTop(trianglePos)) {
			p.y = getTop(trianglePos);
			top = true;
		}
		if (right || top) {
			if (p.x < getLeft(trianglePos)) {
				//もし点が三角形の左端より左にあったら
				p.x = getLeft(trianglePos);
			}
			if (p.y > getBottom(trianglePos)) {
				p.y = getBottom(trianglePos);
			}
			return p;
		}
	}
	if (vx < 0 && vy < 0) {
		//三角形の形状により場合分け
		if (p.x > getRight(trianglePos)) {
			//もし点(円の中心を想定)が三角形の右端より右にあったら
			p.x = getRight(trianglePos);
			right = true;
		}
		//yにも同様の処理をする
		if (p.y > getBottom(trianglePos)) {
			p.y = getBottom(trianglePos);
			bottom = true;
		}
		if (right || bottom) {
			if (p.x < getLeft(trianglePos)) {
				//もし点が三角形の左端より左にあったら
				p.x = getLeft(trianglePos);
			}
			if (p.y < getTop(trianglePos)) {
				p.y = getTop(trianglePos);
			}
			return p;
		}
	}
	//左側にも上側にもない場合、斜辺との最近傍点を求める
	Vector2D endPoint1(trianglePos.x,trianglePos.y+vy), endPoint2(trianglePos.x+vx,trianglePos.y);
	Vector2D hypoVec = endPoint2 - endPoint1;
	Vector2D pointVec = p - endPoint1;			//endpoint1を始点にしてベクトルを張る
	//内積によって垂線の足を求める。
	Vector2D hoot = hypoVec.norm()*pointVec.dot(hypoVec.norm())+endPoint1;
	if (min(endPoint1.x, endPoint2.x) < hoot.x && max(endPoint1.x, endPoint2.x) > hoot.x) {
		return hoot;
	}
	else if (min(endPoint1.x, endPoint2.x) >= hoot.x) {
		if (endPoint1.x < endPoint2.x) return endPoint1;
		else return endPoint2;
	}
	else {
		if (endPoint1.x > endPoint2.x) return endPoint1;
		else return endPoint2;
	}


	return Vector2D(-1.0f,-1.0f);
};