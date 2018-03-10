#ifndef DEF_SHAPEHAVING_H
#define DEF_SHAPEHAVING_H

#include<memory>
#include<vector>
#include"Shape.h"

//当たり判定処理を持つ純粋仮想クラス、「当たり判定を持つ障害物」などを作る時に基底クラスにすると良い
class ShapeHaving{
	//定数
protected:
	static const float pushRate;

	//変数
protected:
	std::shared_ptr<Shape> m_hitJudgeShape;//当たり判定の形状

	//関数
public:
	ShapeHaving(std::shared_ptr<Shape> pShape);
	virtual ~ShapeHaving();

	//仮想関数
	virtual void Move(Vector2D v);//ベクトルvだけ動かす(バトルの時等で用いる)(継承先で当たり判定図形を複数切り替えるのであればオーバーライドする)
	virtual const Shape *GetHitJudgeShape()const;//m_hitJudgeShapeのconstポインタを取得、当たり判定処理に用いる(継承先で当たり判定図形を複数切り替えるのであればオーバーライドする)
	virtual Shape::Fix::Kind SetFix(Shape::Fix::Kind fix)const;//m_hitJudgeShape.m_fixを変更(継承先で当たり判定図形を複数切り替えるのであればオーバーライドする)

	//通常関数
	void ShapeDraw(unsigned int color,int fillFlag,float linethickness=1.0f,Vector2D adjust=Vector2D())const;//当たり判定形状に基づいた描画処理(dx,dyはズレの値)
	void PosDraw(unsigned int color,int fillFlag,float size,Vector2D adjust=Vector2D())const;//m_Posの位置に円を描画する。
	void Warp(Vector2D v);//ベクトルvへ動かす(ステージ編集で用いる)
	void Resize(Vector2D v);//位置ベクトルvを参考に大きさを変更する(ステージ編集で用いる)(m_hitJudgeShapeのものを直接編集)
	void ChangeShape(std::shared_ptr<Shape> pShape);//当たり判定図形を変更する
	bool JudgePointInsideShape(Vector2D point)const;
	float getTop()const;
	float getBottom()const;
	float getLeft()const;
	float getRight()const;
	Vector2D getPos()const;
	Vector2D getResizeVec()const;//m_hitJudgeShapeのものを直接用いる
	Shape::Fix::Kind GetFix()const;//GetHitJudgeShape().m_fixを取得
	const std::vector<const ShapeHaving *> InShapeHavingList(const ShapeHaving  * const * const pShapeHavingVec,const size_t vecSize)const;//pShapeVecにある図形のうちthis内に存在する図形を全てリストアップする
	void UpdatePosition(ShapeHaving * const * const pShapeHavingVec,const size_t vecSize,const size_t updateTimes);//当たり判定による位置補正をupdateTimes回だけ行う。全ての図形に対しまとめて行うのが前提。std::vectorで管理している場合はstd::vector::data()を用いる。
	bool JudgeInShapeRect(const ShapeHaving *pShapeHaving)const;//長方形判定によって中にあるかどうかを判定する

};

#endif // !DEF_SHAPEHAVING_H

#pragma once
