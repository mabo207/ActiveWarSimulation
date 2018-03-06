#ifndef DEF_TERRAIN_H
#define DEF_TERRAIN_H

//地形・障害物についての情報の宣言
#include"BattleObject.h"

class Terrain :public BattleObject {
	//型・列挙体

	//定数

	//変数
protected:
	unsigned int m_color;//地形の塗りつぶしの色(そのうちテクスチャ情報に変わると思われる)
	bool m_breakable;//破壊できるかどうか

	//関数
protected:
	void WriteOutObjectPeculiarInfo(std::ofstream &ofs)const;//オブジェクト特有の情報を返す

public:
	Terrain(const std::shared_ptr<Shape> hitJudgeShape,int gHandle,unsigned int color,bool breakable);
	~Terrain();
	void VDraw(Vector2D point,Vector2D adjust)const;//描画処理
	void VHitProcess(const BattleObject *potherobj);
	std::shared_ptr<BattleObject> VCopy()const;
};


#endif // !DEF_TERRAIN_H
