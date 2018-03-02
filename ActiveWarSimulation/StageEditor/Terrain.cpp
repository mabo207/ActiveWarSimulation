#include"DxLib.h"
#include"Terrain.h"


Terrain::Terrain(const std::shared_ptr<Shape> hitJudgeShape, int gHandle, int kind, unsigned int color, bool breakable)
	:BattleObject(hitJudgeShape,gHandle,kind),m_color(color),m_breakable(breakable){}

Terrain::~Terrain() {}

void Terrain::WriteOutObjectPeculiarInfo(std::ofstream &ofs)const{
	ofs<<"(Terrain)";
}

void Terrain::VDraw(Vector2D point,Vector2D adjust)const{
	m_hitJudgeShape->Draw(adjust,m_color,TRUE,1.0f);
}

void Terrain::VHitProcess(const BattleObject *potherobj) {
	//Œ»Žž“_‚Å‚Í“Á‚É‰½‚à‚µ‚È‚¢
}

std::shared_ptr<BattleObject> Terrain::VCopy()const{
	return std::shared_ptr<BattleObject>(new Terrain(m_hitJudgeShape.get()->VCopy(),m_gHandle,m_kind,m_color,m_breakable));
}
