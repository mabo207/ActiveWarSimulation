#include"DxLib.h"
#include"BattleObject.h"
#include"GraphicControl.h"
#include"StringBuilder.h"

#include"Terrain.h"

BattleObject::~BattleObject(){
	DeleteGraphEX(m_gHandle);
}

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

void BattleObject::WriteOutObjectWholeInfo(std::ofstream &ofs)const{
	//"{"→(オブジェクト特有情報)→","→(図形情報)→"}\n"と出力する
	ofs<<"{";
	WriteOutObjectPeculiarInfo(ofs);
	ofs<<",";
	m_hitJudgeShape->WriteOutShape(ofs);
	ofs<<"}"<<std::endl;
}

std::shared_ptr<BattleObject> BattleObject::CreateObject(const std::string &infostr){
	//CreateRawObject()を用いる
	return std::shared_ptr<BattleObject>(CreateRawObject(infostr));
}

BattleObject *BattleObject::CreateRawObject(const std::string &infostr){
	//strをStringBuilderを用いて分割する
	StringBuilderOld sb(infostr,',','(',')',false,true);
	//strの解釈。sb.m_vec[0]:オブジェクトの種類 strVec[1]:当たり判定図形の情報
	if(sb.m_vec.size()<2){
		return nullptr;
	}
	//当たり判定図形の生成
	std::shared_ptr<Shape> pShape=Shape::CreateShape(sb.m_vec[1].GetString());
	//オブジェクトの生成
	BattleObject *pb(nullptr);
	if(pShape.get()!=nullptr){
		//当たり判定図形がnullptrならオブジェクト生成は行わずにnullptrを返す。
		const std::string objectname=sb.m_vec[0].GetString();
		if(objectname==Type::GetStr(Type::e_terrain)){
			pb=new Terrain(pShape,-1,GetColor(128,128,128),true);
		}
	}

	return pb;
}
