#include"Shape.h"

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

void Shape::Update(const std::vector<std::shared_ptr<Shape>> &pShapeVec){
	if(m_fix==Fix::e_dynamic){
		//m_fixによって動くかどうかを決める
		//この関数が呼ばれた際図形が存在するのは仮移動先である。仮移動先において、自分と重なっている図形を探し、位置をずらす。
		//重なっている図形の外に出るように押し出し処理をする
		const float pushRate=Shape::pushRate;//図形が重ならなくなるのにぴったりの距離に対し、押し出す距離の割合。1フレーム内で完全に押し出すと、図形に挟まった時に不自然な挙動をする。
		for(const std::shared_ptr<const Shape> &pShape:pShapeVec){
			//当たり判定に影響を与える図形以外に対して押し出し処理を行う
			if(pShape->m_fix!=Fix::e_ignore){
				//各図形に対して完全に押し出せる距離を求める
				Vector2D pushVec=CalculatePushVec(pShape.get());
				//押し出し距離の一定の割合のベクトルだけ押し出す
				m_position+=pushVec*pushRate;
			}
		}
	}
}

std::shared_ptr<Shape> Shape::CreateShape(const std::string &infostr){
	//「種類,位置,初期固定,図形情報」の順。それぞれの要素を抽出する。
	std::vector<std::string> strVec;//分割したもの
	char ch;//読み込んだ1文字を一時格納する
	int tokenCount=0;//読み込んだ(の個数-読み込んだ)の個数。0より大きいならstrに追加。0未満にはならないようにする。
	std::string str;//読み込んだ()内文字列
	str.reserve(40);//処理速度を速めるためにreserveはしておく。職人技になる。
	for(std::string::const_iterator it=infostr.begin(),ite=infostr.end();it!=ite;it++){
		ch=*it;
		//トークンは'(',')',','の3つ
		if(ch==',' && tokenCount<=0){
			//オブジェクト読み込み開始、しかし()内のものは無視する
			
		} else if(ch==')'){
			//トークンのcountを調整
			if(tokenCount>=0){
				//負の個数になるトークンは無視する
				tokenCount--;
				if(tokenCount>0){
					//トークンのcountが正なら読み込みを続ける
					str.push_back(ch);
				} else{
					//トークンのcountが0になったら()内読み込みは終了、strVecに格納
					strVec.push_back(str);
					str.clear();
				}
			}
		} else{
			//tokenCountが0より大きいならstrに追加
			if(tokenCount>0){
				str.push_back(ch);
			}
		}
	}
	//strの解釈。strVec[0]:オブジェクトの種類 strVec[1]:当たり判定図形の情報

	return std::shared_ptr<Shape>(nullptr);
}
