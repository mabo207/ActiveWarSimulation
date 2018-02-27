#include"MyShape.h"
#include"MyRectangle.h"
#include"MyCircle.h"
#include"MyAngledTriangle.h"
#include "ToolsLib.h"

MyShape::HitInfo MyShape::HitJudge(const MyShape *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const {
	//ダウンキャストしてどのHitJudgeを呼べば良いかを判断する
	{
		const MyCircle *pCircle = dynamic_cast<const MyCircle *>(pshape);
		if (pCircle != nullptr) {
			return VHitJudge(pCircle,aMyPos,aOtherPos);

		}
	}
	{
		const MyRectangle *pRectangle = dynamic_cast<const MyRectangle *>(pshape);
		if (pRectangle != nullptr) {

			return VHitJudge(pRectangle, aMyPos, aOtherPos);

		}
	}
	{
		const MyAngledTriangle *pAngledTriangle = dynamic_cast<const MyAngledTriangle *>(pshape);
		if (pAngledTriangle != nullptr) {
			return VHitJudge(pAngledTriangle, aMyPos, aOtherPos);

		}
	}

	return HitInfo(false,Vector2D(0.0f,0.0f));
}

//当たり判定の実体
MyShape::HitInfo MyShape::HitJudge(const MyCircle *pshape1, const MyCircle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos) {
	if ((pshape1->r + pshape2->r)*(pshape1->r + pshape2->r) >= (aMyPos.x - aOtherPos.x)*(aMyPos.x - aOtherPos.x) + (aMyPos.y - aOtherPos.y)*(aMyPos.y - aOtherPos.y)) {
		//めり込み量を計算
		float fixSize = (pshape1->r+pshape2->r-(aMyPos - aOtherPos).size());
		//補正ベクトルを計算(自分向きのベクトルを返す、これを足すことで補正ができる)
		Vector2D fixVec = (aMyPos - aOtherPos).norm()*fixSize;
		return HitInfo(true,fixVec);
	}
	return HitInfo(false,Vector2D(0.0f,0.0f));
}

MyShape::HitInfo MyShape::HitJudge(const MyCircle *pshape1, const MyRectangle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos) {
	//最近傍点を求める
	Vector2D nearestPoint = pshape2->GetNearestPoint(aOtherPos, aMyPos);
	if ((aMyPos - nearestPoint).sqSize() < pshape1->r*pshape1->r) {
		//円の中心から最近傍点までの距離**2<半径**2なら衝突している
		//めり込み量を計算
		float fixSize = pshape1->r-(aMyPos - nearestPoint).size();
		//補正ベクトルを計算(自分向きのベクトルを返す、これを足すことで補正ができる)
		Vector2D fixVec = (aMyPos - nearestPoint).norm()*fixSize*(float)0.97;
		return HitInfo(true,fixVec);
	}

	return HitInfo(false, Vector2D(0.0f, 0.0f));
}

MyShape::HitInfo MyShape::HitJudge(const MyCircle *pshape1, const MyAngledTriangle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos) {
	Vector2D nearestPoint = pshape2->GetNearestPoint(aOtherPos, aMyPos);
	if ((aMyPos - nearestPoint).sqSize() < pshape1->r*pshape1->r) {
		//円の中心から最近傍点までの距離**2<半径**2なら衝突している
		//めり込み量を計算
		float fixSize = pshape1->r - (aMyPos - nearestPoint).size();
		//補正ベクトルを計算(自分向きのベクトルを返す、これを足すことで補正ができる)
		Vector2D fixVec = (aMyPos - nearestPoint).norm()*fixSize;
		return HitInfo(true, fixVec);
	}
	return HitInfo(false, Vector2D(0.0f, 0.0f));
}

//当面は動くオブジェクトは円のみとするので今の所はfalseを返すままにしておきます
MyShape::HitInfo MyShape::HitJudge(const MyRectangle *pshape1, const MyRectangle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos) {
	return HitInfo(false, Vector2D(0.0f, 0.0f));
}

MyShape::HitInfo MyShape::HitJudge(const MyRectangle *pshape1, const MyAngledTriangle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos) {
	return HitInfo(false, Vector2D(0.0f, 0.0f));
}

MyShape::HitInfo MyShape::HitJudge(const MyAngledTriangle *pshape1, const MyAngledTriangle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos) {
	return HitInfo(false, Vector2D(0.0f, 0.0f));
}

