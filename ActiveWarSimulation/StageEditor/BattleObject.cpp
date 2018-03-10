#include"DxLib.h"
#include"BattleObject.h"

#include"Terrain.h"

std::string BattleObject::Type::GetStr(Kind kind){
	switch(kind){
	case(e_terrain):
		return "Terrain";
	case(e_unit):
		return "Unit";
	default:
		return "";
	}
}

void BattleObject::VDraw(Vector2D adjust)const{
	VDraw(m_hitJudgeShape->GetPosition(),adjust);
}

void BattleObject::ShapeDraw(unsigned int color,int fillFlag,float lineThickness,Vector2D adjust)const{
	m_hitJudgeShape->Draw(adjust,color,fillFlag,lineThickness);
}

void BattleObject::PosDraw(unsigned int color,int fillFlag,float size,Vector2D adjust)const{
	Vector2D pos=adjust+m_hitJudgeShape->GetPosition();
	DrawCircleAA(pos.x,pos.y,size,color,fillFlag);
}

void BattleObject::Move(Vector2D v){
	m_hitJudgeShape->Move(v);
}

void BattleObject::Warp(Vector2D v){
	m_hitJudgeShape->Move(v-m_hitJudgeShape->GetPosition());
}

void BattleObject::Resize(Vector2D v){
	//基準点からマウスを結んだベクトルを引数に渡す
	m_hitJudgeShape.get()->Resize(v-m_hitJudgeShape->GetPosition());
}

void BattleObject::ChangeShape(std::shared_ptr<Shape> pShape){
	m_hitJudgeShape=pShape;
}

bool BattleObject::JudgePointInsideShape(Vector2D point)const{
	return m_hitJudgeShape.get()->VJudgePointInsideShape(point);
}

float BattleObject::getTop()const {
	return m_hitJudgeShape->GetLeftTop().y;
}

float BattleObject::getBottom()const {
	return m_hitJudgeShape->GetRightBottom().y;
}

float BattleObject::getLeft()const {
	return m_hitJudgeShape->GetLeftTop().x;
}

float BattleObject::getRight()const {
	return m_hitJudgeShape->GetRightBottom().x;
}

Vector2D BattleObject::getPos()const {
	return m_hitJudgeShape->GetPosition();
}

std::shared_ptr<Shape> BattleObject::getHitJudgeShape()const {
	return m_hitJudgeShape;
}

Vector2D BattleObject::getResizeVec()const{
	//m_hitJudgeShapeのものはオブジェクトの位置を考慮していないのでそれを足して返す
	return m_hitJudgeShape.get()->GetRetResize()+m_hitJudgeShape->GetPosition();
}

void BattleObject::WriteOutObjectWholeInfo(std::ofstream &ofs)const{
	//"{"→(オブジェクト特有情報)→","→(図形情報)→"}\n"と出力する
	ofs<<"{";
	WriteOutObjectPeculiarInfo(ofs);
	ofs<<",";
	m_hitJudgeShape->WriteOutShape(ofs);
	ofs<<"}"<<std::endl;
}

std::shared_ptr<BattleObject> BattleObject::CreateObject(const std::string &infostr){
	//strをStringBuilderを用いて分割する
	StringBuilder sb(infostr,',','(',')',false,true);
	//strの解釈。sb.m_vec[0]:オブジェクトの種類 strVec[1]:当たり判定図形の情報
	if(sb.m_vec.size()<2){
		return std::shared_ptr<BattleObject>(nullptr);
	}
	//当たり判定図形の生成
	std::shared_ptr<Shape> pShape=Shape::CreateShape(sb.m_vec[1].GetString());
	//オブジェクトの生成
	std::shared_ptr<BattleObject> pb(nullptr);
	if(pShape.get()!=nullptr){
		//当たり判定図形がnullptrならオブジェクト生成は行わずにnullptrを返す。
		const std::string objectname=sb.m_vec[0].GetString();
		if(objectname==Type::GetStr(Type::e_terrain)){
			pb=std::shared_ptr<BattleObject>(new Terrain(pShape,-1,GetColor(128,128,128),true));
		}
	}

	return pb;
}
