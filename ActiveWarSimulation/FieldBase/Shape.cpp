#include"Shape.h"
#include"Circle.h"
#include"Edge.h"

//---------------------Shape::Type---------------------
std::string Shape::Type::GetStr(Kind kind){
	switch(kind){
	case(e_circle):
		return "Circle";
	case(e_edge):
		return "Edge";
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
const float Shape::pushRate=0.4f;
const char Shape::spliter=',';
const char Shape::beginer='(';
const char Shape::ender=')';

Shape::Shape(Vector2D position,Type::Kind type,Fix::Kind fix)
	:m_position(position),m_type(type),m_fix(fix){}

Shape::~Shape(){}

void Shape::Move(Vector2D displacement){
	m_position+=displacement;
}

const std::vector<std::shared_ptr<const Shape>> Shape::InShapeList(const std::vector<std::shared_ptr<const Shape>> &pShapeVec)const{
	std::vector<std::shared_ptr<const Shape>> list;
	const Vector2D v=Vector2D();
	for(const std::shared_ptr<const Shape> &pShape:pShapeVec){
		if(CalculatePushVec(pShape.get())!=v){
			list.push_back(pShape);
		}
	}
	return list;
}

void Shape::Update(const std::vector<std::shared_ptr<Shape>> &pShapeVec,size_t updateTimes){
	if(m_fix==Fix::e_dynamic){
		//m_fixによって動くかどうかを決める
		//この関数が呼ばれた際図形が存在するのは仮移動先である。仮移動先において、自分と重なっている図形を探し、位置をずらす。
		//まず重なっている図形を全てリストアップする
		std::vector<std::shared_ptr<const Shape>> inShapeList;
		for(const std::shared_ptr<const Shape> &pShape:pShapeVec){
			//当たり判定に影響を与える図形以外に対して押し出し処理を行う
			if(pShape->m_fix!=Fix::e_ignore){
				//図形をぴったり収める長方形を利用して簡易的な当たり判定を行う
				if(JudgeInShapeRect(pShape.get())){
					//押し出しする図形に追加
					inShapeList.push_back(pShape);
				}
			}
		}
		//簡易的当たり判定によって判定できる、重なっている図形の外に出るように押し出し処理をする
		for(size_t i=0;i<updateTimes;i++){
			for(const std::shared_ptr<const Shape> &pShape:inShapeList){
				//各図形に対して完全に押し出せる距離を求める
				Vector2D pushVec=CalculatePushVec(pShape.get());
				//押し出し距離の一定の割合のベクトルだけ押し出す
				m_position+=pushVec*Shape::pushRate;
			}
		}
	}
}

bool Shape::JudgeInShapeRect(const Shape *pShape)const{
	return this->GetLeftTop().x<=pShape->GetRightBottom().x && this->GetRightBottom().x>=pShape->GetLeftTop().x && this->GetLeftTop().y<=pShape->GetRightBottom().y && this->GetRightBottom().y>=pShape->GetLeftTop().y;
}

std::shared_ptr<Shape> Shape::CreateShape(const std::string &infostr){
	//「種類,位置,初期固定,図形情報」の順。それぞれの要素を抽出する。
	//全て()が集合文字、,が区切り文字なので一括で分割する。
	StringBuilder sb(infostr,spliter,beginer,ender,true,true);
	//strの解釈。sb.m_vec[0]:オブジェクトの種類 sb.m_vec[1]:当たり判定図形の情報((x,y)形式) sb.m_vec[2]:初期固定 sb.m_vec[3]:図形情報
	std::shared_ptr<Shape> pShape(nullptr);
	try{
		//位置の生成(共通処理)
		Vector2D pos(std::stof(sb.m_vec.at(1).m_vec.at(0).GetString()),std::stof(sb.m_vec.at(1).m_vec.at(1).GetString()));
		//初期固定の生成
		const std::string fixName=sb.m_vec.at(2).GetString();
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
		if(sb.m_vec.at(0).GetString()==Type::GetStr(Type::e_circle)){
			//円の生成を行う。m_vec[3]の中身は半径の値のみ。
			float r=std::stof(sb.m_vec.at(3).GetString());
			pShape=std::shared_ptr<Shape>(new Circle(pos,r,fix));
		} else if(sb.m_vec.at(0).GetString()==Type::GetStr(Type::e_edge)){
			//線分の生成を行う。m_vec[3]の中身は(x,y)形式の座標のみ。
			Vector2D vec(std::stof(sb.m_vec.at(3).m_vec.at(0).GetString()),std::stof(sb.m_vec.at(3).m_vec.at(1).GetString()));
			pShape=std::shared_ptr<Shape>(new Edge(pos,vec,fix));
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
