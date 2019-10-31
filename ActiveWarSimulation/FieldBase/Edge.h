#ifndef DEF_EDGE_H
#define DEF_EDGE_H

#include"Shape.h"

//線分を表すクラス
class Edge:public Shape{
	//型・列挙体

	//定数

	//変数
protected:
	Vector2D m_vec;//始点から終点へ向かうベクトル

	//関数
protected:
	Vector2D CalculateParpendicularVec(Vector2D point)const;//pointからこの直線に垂線を下ろした時、直線からpointに向けた方向の垂線ベクトルを求める
	
public:
	Edge(Vector2D begin,Vector2D vec,Fix::Kind fix);
	~Edge();
	std::shared_ptr<Shape> VCopy()const;//内容が同じでポインタの位置のみが異なるオブジェクトのポインタを返す
	Vector2D GetBeginPoint()const{
		return m_position;
	}
	Vector2D GetEndPoint()const{
		//終点の位置を返す
		return m_position+m_vec;
	}
	Vector2D CalculatePushVec(const Shape *pShape)const;//pShapeとthisが重なっているか判定し、押し出すベクトルを返す。重なっていない場合はVector2D(0,0)が返される。
	void BrokenDraw(Vector2D adjust,unsigned int color,float lineThickness,float drawSize,float eraseSize)const;//破線描画
	void BrokenDraw(Vector2D point,Vector2D adjust,float exRate,unsigned int color,float lineThickness,float drawSize,float eraseSize)const;//破線描画

	//純粋仮想関数のオーバーライド
	void Draw(Vector2D point,Vector2D adjust,float exRate,unsigned int color,int fillFlag,float lineThickness)const;
	bool PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const;//thisとpShapeが重なっているか判定し、重なっている場合はparentObjを移動させtrueを返す。
	bool JudgeInShape(const Shape *pShape)const;//this内にpShapeがあるかどうかの判定
	Vector2D GetLeftTop()const;//左上の座標を求める
	Vector2D GetRightBottom()const;//右下の座標を求める
	void RecordLatticePointInShape(std::vector<int> &latticeInShape,const size_t xNum,const size_t yNum,const size_t squareWidth,const size_t squareHeight,int index)const;//この図形内部にある格子点の配列を全てindexにする
	//エディタ用
	bool VJudgePointInsideShape(Vector2D point)const;//図形内に点があるかどうかの判定、CalculatePushVecを用いるより高速に実装できるので関数を分ける
	Vector2D VGetNearEndpoint(Vector2D point,float capacity)const;//pointが端点に近い(距離がcapacity以内)場合、その端点を返す
	void Resize(Vector2D v);//図形を表現するベクトルを用いて図形の大きさを変更する
	Vector2D GetRetResize()const;//Resizeの逆関数。引数rでResizeすると現在の図形になるようなrを返す。
	//読み込み・書き出し用関数
	void WriteOutShape(std::ofstream &ofs)const;//書き出し用関数
};


#endif // !DEF_EDGE_H
#pragma once
