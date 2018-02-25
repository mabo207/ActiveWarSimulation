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
	Vector2D GetBeginPoint()const{
		return m_position;
	}
	Vector2D GetEndPoint()const{
		//終点の位置を返す
		return m_position+m_vec;
	}
	void Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
	Vector2D CalculatePushVec(const Shape *pShape)const;//pShapeとthisが重なっているか判定し、押し出すベクトルを返す。重なっていない場合はVector2D(0,0)が返される。
};


#endif // !DEF_EDGE_H
#pragma once
