#include"DxLib.h"
#include"Terrain.h"


Terrain::Terrain(const std::shared_ptr<Shape> hitJudgeShape, int gHandle, unsigned int color, bool breakable)
	:BattleObject(Type::e_terrain,hitJudgeShape,gHandle),m_color(color),m_breakable(breakable){}

Terrain::~Terrain() {}

void Terrain::WriteOutObjectPeculiarInfo(std::ofstream &ofs)const{
	ofs<<"("<<Type::GetStr(m_type)<<")";
}

void Terrain::VDraw(Vector2D point,Vector2D adjust)const{
	m_hitJudgeShape->Draw(adjust,m_color,TRUE,5.0f);
}

void Terrain::VHitProcess(const BattleObject *potherobj) {
	//Œ»Žž“_‚Å‚Í“Á‚É‰½‚à‚µ‚È‚¢
}

std::shared_ptr<BattleObject> Terrain::VCopy()const{
	return std::shared_ptr<BattleObject>(new Terrain(m_hitJudgeShape.get()->VCopy(),m_gHandle,m_color,m_breakable));
}
