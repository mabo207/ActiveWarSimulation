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
	std::vector<std::array<size_t,3>> m_triangleSet;//三角形分割の結果が格納されている。回転・拡大縮小しても変更する必要はない。

	//関数
protected:
	void DivideTriangle(std::vector<std::array<size_t,3>> *triangleSet)const;
	
public:
	MyPolygon(Vector2D begin,std::vector<Vector2D> points,Fix::Kind fix);
	~MyPolygon();
	std::vector<Vector2D> GetAllEdgeVecs()const;//m_edgeVecsに終点→始点へのベクトルを加え、全ての辺のベクトルを返す。
	void CalculateAllPointPosition(std::vector<Vector2D> *vertexPos)const;//全ての頂点の座標をvertexPosに格納する

	//純粋仮想関数
	std::shared_ptr<Shape> VCopy()const;//内容が同じでポインタの位置のみが異なるオブジェクトのポインタを返す
	void Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
	//Vector2D CalculatePushVec(const Shape *pShape)const;//pShapeとthisが重なっているか判定し、押し出すベクトルを返す。重なっていない場合はVector2D(0,0)が返される。
	bool PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const;//thisとpShapeが重なっているか判定し、重なっている場合はparentObjを移動させtrueを返す。
	bool JudgeInShape(const Shape *pShape)const;//this内にpShapeがあるかどうかの判定
	Vector2D GetLeftTop()const;//左上の座標を求める
	Vector2D GetRightBottom()const;//右下の座標を求める
	void RecordLatticePointInShape(std::vector<int> &latticeInShape,const size_t xNum,const size_t yNum,const size_t squareWidth,const size_t squareHeight,int index)const;//この図形内部にある格子点の配列を全てindexにする
	//エディタ用の純粋仮想関数
	bool VJudgePointInsideShape(Vector2D point)const;//図形内に点があるかどうかの判定、CalculatePushVecを用いるより高速に実装できるので関数を分ける
	Vector2D VGetNearEndpoint(Vector2D point,float capacity)const;//pointが端点に近い(距離がcapacity以内)場合、その端点を返す
	void Resize(Vector2D v);//図形を表現するベクトルを用いて図形の大きさを変更する
	Vector2D GetRetResize()const;//Resizeの逆関数。引数rでResizeすると現在の図形になるようなrを返す。
	//読み込み・書き出し用仮想関数
	void WriteOutShape(std::ofstream &ofs)const;//書き出し用関数

};


#endif // !DEF_MYPOLYGON_H

