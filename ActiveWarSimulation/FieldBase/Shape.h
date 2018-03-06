#ifndef DEF_SHAPE_H
#define DEF_SHAPE_H

#include"ToolsLib.h"
#include<vector>
#include<memory>
#include<iostream>
#include<fstream>

//当たり判定図形を指す純粋仮想関数
class Shape{
	//列挙体・型
public:
	struct Type{
		enum Kind{
			e_circle,
			e_edge,
			END
		};
		static Kind link(int num){
			if(num>=0 && num<END){
				return static_cast<Kind>(num);
			}
			return END;
		}
	};
	struct Fix{
		//自オブジェクト移動時は衝突判定があると必ず移動場所が補正される
		enum Kind{
			e_dynamic,//他オブジェクトの移動時に影響を与える、他オブジェクト移動時において衝突判定があると位置が補正される
			e_static,//他オブジェクトの移動時に影響を与える、他オブジェクト移動時において衝突判定があっても位置が補正されない
			e_ignore,//他オブジェクトの移動時に影響を与えない
			END
		};
		static Kind link(int num){
			if(num>=0 && num<END){
				return static_cast<Kind>(num);
			}
			return END;
		}
		static std::string GetStr(Kind kind);
	};

	//定数
public:
	static const float pushRate;//当たり判定処理を行う際、押し出す距離の割合

	//変数
protected:
	Vector2D m_position;//図形の位置
	Type::Kind m_type;//図形の種類

public:
	Fix::Kind m_fix;//図形が固定されているかどうか(おそらく各キャラの行動順が回るたびに設定し直される)

	//関数
protected:

public:
	Shape(Vector2D position,Type::Kind type,Fix::Kind fix);
	virtual ~Shape();
	Vector2D GetPosition()const{
		return m_position;
	}
	Type::Kind GetType()const{
		return m_type;
	}
	void Move(Vector2D displacement);//オブジェクトを移動させる。数フレームかけて位置補正を行うため、当たり判定による位置の補正はMove()ではなくUpdate()によって行う
	const std::vector<std::shared_ptr<const Shape>> InShapeList(const std::vector<std::shared_ptr<const Shape>> &pShapeVec)const;//pShapeVecにある図形のうちthis内に存在する図形を全てリストアップする
	void Update(const std::vector<std::shared_ptr<Shape>> &pShapeVec);//当たり判定による位置補正を行う。全ての図形に対しまとめて行うのが前提。
	//純粋仮想関数
	virtual std::shared_ptr<Shape> VCopy()const=0;//内容が同じでポインタの位置のみが異なるオブジェクトのポインタを返す
	virtual void Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineTickness=1.0f)const=0;
	virtual Vector2D CalculatePushVec(const Shape *pShape)const=0;//pShapeとthisが重なっているか判定し、押し出すベクトルを返す。重なっていない場合はVector2D(0,0)が返される。
	virtual Vector2D GetLeftTop()const=0;//左上の座標を求める
	virtual Vector2D GetRightBottom()const=0;//右下の座標を求める
	//エディタ用の純粋仮想関数
	virtual bool VJudgePointInsideShape(Vector2D point)const=0;//図形内に点があるかどうかの判定、CalculatePushVecを用いるより高速に実装できるので関数を分ける
	virtual void Resize(Vector2D v)=0;//図形を表現するベクトルを用いて図形の大きさを変更する
	virtual Vector2D GetRetResize()const=0;//Resizeの逆関数。引数rでResizeすると現在の図形になるようなrを返す。
	//読み込み・書き出し用仮想関数
	virtual void WriteOutShape(std::ofstream &ofs)const=0;//書き出し用関数
	static std::shared_ptr<Shape> CreateShape(const std::string &infostr);//文字列からShapeを生成する
};

#endif // !DEF_SHAPE_H
#pragma once
