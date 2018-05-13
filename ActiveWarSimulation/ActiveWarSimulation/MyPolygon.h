#ifndef DEF_MYPOLYGON_H
#define DEF_MYPOLYGON_H

#include<vector>
#include"Edge.h"

//多角形を示す図形
class MyPolygon:public Shape{
	//列挙体・型
public:

	//定数

	//変数
protected:
	std::vector<Vector2D> m_edgeVecs;//辺を表すベクトル。m_positionにm_edgeVecsの先頭から加算していくと、全ての点の座標を求められる。終端点->m_positionのベクトルは自明なため、点の数より要素数が1少ない。

	//関数
protected:
	std::vector<Vector2D> GetAllEdgeVecs()const;//m_edgeVecsに終点→始点へのベクトルを加え、全ての辺のベクトルを返す。

public:
	MyPolygon(Vector2D begin,std::vector<Vector2D> points,Fix::Kind fix);
	~MyPolygon();

	//純粋仮想関数
	std::shared_ptr<Shape> VCopy()const;//内容が同じでポインタの位置のみが異なるオブジェクトのポインタを返す
	void Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
	Vector2D CalculatePushVec(const Shape *pShape)const;//pShapeとthisが重なっているか判定し、押し出すベクトルを返す。重なっていない場合はVector2D(0,0)が返される。
	Vector2D GetLeftTop()const;//左上の座標を求める
	Vector2D GetRightBottom()const;//右下の座標を求める
	//エディタ用の純粋仮想関数
	bool VJudgePointInsideShape(Vector2D point)const;//図形内に点があるかどうかの判定、CalculatePushVecを用いるより高速に実装できるので関数を分ける
	Vector2D VGetNearEndpoint(Vector2D point,float capacity)const;//pointが端点に近い(距離がcapacity以内)場合、その端点を返す
	void Resize(Vector2D v);//図形を表現するベクトルを用いて図形の大きさを変更する
	Vector2D GetRetResize()const;//Resizeの逆関数。引数rでResizeすると現在の図形になるようなrを返す。
	//読み込み・書き出し用仮想関数
	void WriteOutShape(std::ofstream &ofs)const;//書き出し用関数

};


#endif // !DEF_MYPOLYGON_H

