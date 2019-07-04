#include"Shape.h"
#include"Circle.h"
#include"Edge.h"
#include"MyPolygon.h"
#include"StringBuilder.h"

//---------------------Shape::Type---------------------
std::string Shape::Type::GetStr(Kind kind){
	switch(kind){
	case(e_circle):
		return "Circle";
	case(e_edge):
		return "Edge";
	case(e_polygon):
		return "Polygon";
	default:
		return "";
	}
}

//---------------------Shape::Fix---------------------
std::string Shape::Fix::GetStr(Kind kind){
	switch(kind){
	case(e_dynamic):
		return "dynamic";
	case(e_static):
		return "static";
	case(e_ignore):
		return "ignore";
	default:
		return "";
	}
}

//---------------------Shape---------------------
const char Shape::spliter=',';
const char Shape::beginer='(';
const char Shape::ender=')';

Shape::Shape(Vector2D position,Type::Kind type,Fix::Kind fix)
	:m_position(position),m_type(type),m_fix(fix){}

Shape::~Shape(){}

void Shape::Move(Vector2D displacement){
	m_position+=displacement;
}

void Shape::Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	Draw(m_position,adjust,color,fillFlag,lineThickness);
}

std::shared_ptr<Shape> Shape::CreateShape(StringBuilder &info){
	//「種類,位置,初期固定,図形情報」の順。それぞれの要素を抽出する。
	//全て()が集合文字、,が区切り文字なので一括で分割する。
	info.Split(spliter,beginer,ender);
	//strの解釈。info.m_vec[0]:オブジェクトの種類 info.m_vec[1]:当たり判定図形の情報((x,y)形式) info.m_vec[2]:初期固定 info.m_vec[3]:図形情報
	std::shared_ptr<Shape> pShape(nullptr);
	try{
		//位置の生成(共通処理)
		Vector2D pos(std::stof(info.m_vec.at(1).m_vec.at(0).GetString()),std::stof(info.m_vec.at(1).m_vec.at(1).GetString()));
		//初期固定の生成
		const std::string fixName=info.m_vec.at(2).GetString();
		Fix::Kind fix;
		if(fixName==Fix::GetStr(Fix::e_dynamic)){
			fix=Fix::e_dynamic;
		} else if(fixName==Fix::GetStr(Fix::e_ignore)){
			fix=Fix::e_ignore;
		} else if(fixName==Fix::GetStr(Fix::e_static)){
			fix=Fix::e_static;
		} else{
			//変換できないので例外処理
			throw std::invalid_argument("");
		}
		//図形各自の処理
		if(info.m_vec.at(0).GetString()==Type::GetStr(Type::e_circle)){
			//円の生成を行う。m_vec[3]の中身は半径の値のみ。
			float r=std::stof(info.m_vec.at(3).GetString());
			pShape=std::shared_ptr<Shape>(new Circle(pos,r,fix));
		} else if(info.m_vec.at(0).GetString()==Type::GetStr(Type::e_edge)){
			//線分の生成を行う。m_vec[3]の中身は(x,y)形式の座標のみ。
			Vector2D vec(std::stof(info.m_vec.at(3).m_vec.at(0).GetString()),std::stof(info.m_vec.at(3).m_vec.at(1).GetString()));
			pShape=std::shared_ptr<Shape>(new Edge(pos,vec,fix));
		} else if(info.m_vec.at(0).GetString()==Type::GetStr(Type::e_polygon)){
			//多角形の生成を行う。m_vec[3]の中身は()で全体が囲われ、その中に(x,y)形式の座標が格納されている
			const size_t size=info.m_vec.at(3).m_vec.size();
			std::vector<Vector2D> points;
			points.reserve(size);
			for(size_t i=0;i<size;i++){
				//頂点座標を全て格納
				points.push_back(Vector2D(std::stof(info.m_vec.at(3).m_vec.at(i).m_vec.at(0).GetString()),std::stof(info.m_vec.at(3).m_vec.at(i).m_vec.at(1).GetString())));
			}
			pShape=std::shared_ptr<Shape>(new MyPolygon(pos,points,fix));
		}
	} catch(std::invalid_argument){
		//string→floatの初期化をミスった時
		//string→Fix::Kindの変換をミスった時
	} catch(std::out_of_range){
		//string→数値の変換において範囲外になってしまった時。
		//また配列外参照をした時
	}
	return pShape;
}
