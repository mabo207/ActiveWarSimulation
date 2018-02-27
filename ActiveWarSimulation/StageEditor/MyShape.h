#ifndef DEF_MYSHAPE_H
#define DEF_MYSHAPE_H

#include<memory>
#include"ToolsLib.h"

class MyCircle;
class MyRectangle;
class MyAngledTriangle;

//形状を定義するインターフェース
class MyShape {
	//型・列挙体
public:
	//定数
	struct HitInfo {
		//当たり判定を行った後に返す情報
		bool judge;
		Vector2D fixVec;	//補正ベクトル
		
				//コンストラクタ
		HitInfo(bool i_judge,Vector2D v)
			:judge(i_judge),fixVec(Vector2D(v.x,v.y)) {}
		
	};
	//変数

	//関数
public:
	virtual void VDraw(float x,float y,unsigned int color,int fillFlag,int textureHandle)const=0;//形状を描画する
	virtual std::shared_ptr<MyShape> VCopy()const=0;//内容が同じでポインタの位置のみが異なるオブジェクトのポインタを返す
	virtual void Resize(Vector2D v)=0;//図形を表現するベクトルを用いて図形の大きさを変更する
	virtual Vector2D GetRetResize()const=0;//Resizeの逆関数。引数rでResizeすると現在の図形になるようなrを返す。

	//図形内に点があるかどうかの判定
	virtual bool VJudgePointInsideMyShape(const Vector2D point,const Vector2D shapePos)const=0;
	
	//当たり判定関数、引数に座標情報を追加
	HitInfo HitJudge(const MyShape *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const;
	virtual HitInfo VHitJudge(const MyCircle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const = 0;
	virtual HitInfo VHitJudge(const MyRectangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const = 0;
	virtual HitInfo VHitJudge(const MyAngledTriangle *pshape, const Vector2D aMyPos, const Vector2D aOtherPos)const = 0;

	//当たり判定関数の実体
	static HitInfo HitJudge(const MyCircle *pshape1, const MyCircle *pshape2,const Vector2D aMyPos, const Vector2D aOtherPos);
	static HitInfo HitJudge(const MyCircle *pshape1, const MyRectangle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos);
	static HitInfo HitJudge(const MyCircle *pshape1, const MyAngledTriangle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos);
	static HitInfo HitJudge(const MyRectangle *pshape1, const MyRectangle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos);
	static HitInfo HitJudge(const MyRectangle *pshape1, const MyAngledTriangle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos);
	static HitInfo HitJudge(const MyAngledTriangle *pshape1, const MyAngledTriangle *pshape2, const Vector2D aMyPos, const Vector2D aOtherPos);

	//外接四角形の上下左右を取得(4分木に登録するため)
	virtual float getTop(Vector2D aPos)const = 0;
	virtual float getBottom(Vector2D aPos)const = 0;
	virtual float getLeft(Vector2D aPos)const = 0;
	virtual float getRight(Vector2D aPos)const = 0;
};

#endif // !DEF_MYSHAPE_H
