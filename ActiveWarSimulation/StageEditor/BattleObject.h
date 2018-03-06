#ifndef DEF_BATTLEOBJECT_H
#define DEF_BATTLEOBJECT_H

//インクルードファイル
#include<memory>
#include<iostream>
#include<fstream>
#include"ToolsLib.h"
#include"Shape.h"


//別の場所で宣言しておくこと。
//struct Position2D;//一般的な位置を表す構造体


//バトル画面におけるオブジェクトの基底クラス
class BattleObject {
	//型
public:
	struct Type{
		//オブジェクトの種類
		enum Kind{
			e_terrain,//障害物
			e_unit,//ユニット
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

	//変数
protected:
	Type::Kind m_type;//オブジェクトの種類
	int m_gHandle;//オブジェクトのグラフィックハンドル
	//protectedに戻しました(getterを作ったため)
	//Vector2D m_pos;//オブジェクトの位置(m_hitJudgeShapeが持つ)
	std::shared_ptr<Shape> m_hitJudgeShape;//当たり判定の形状

public:
	//補正される側か？(プレイヤーキャラか？)
	bool fixFlag;

	//関数
protected:
	//仮想関数
	virtual void WriteOutObjectPeculiarInfo(std::ofstream &ofs)const=0;//オブジェクト特有の情報を返す

public:
	//コンストラクタとデストラクタ
	BattleObject(Type::Kind type,const std::shared_ptr<Shape> hitJudgeShape,int gHandle)
		:m_hitJudgeShape(hitJudgeShape),m_gHandle(gHandle),fixFlag(false){}
	virtual ~BattleObject() {}

	
	//仮想関数
	void VDraw(Vector2D adjust=Vector2D(0.0f,0.0f))const;//描画処理(位置ずらし含める)
	virtual void VDraw(Vector2D point,Vector2D adjust)const=0;//描画処理(任意座標に描画)
	virtual void VHitProcess(const BattleObject *potherobj)=0;//何かに当たった時の処理
	virtual std::shared_ptr<BattleObject> VCopy()const=0;//同じオブジェクトを複製する(ポインタのみ異なる)

	//通常関数
	void ShapeDraw(unsigned int color,int fillFlag,Vector2D adjust=Vector2D())const;//当たり判定形状に基づいた描画処理(dx,dyはズレの値)
	void PosDraw(unsigned int color,int fillFlag,float size,Vector2D adjust=Vector2D())const;//m_Posの位置に円を描画する。
	void Move(Vector2D v);//ベクトルvだけ動かす(バトルの時等で用いる)
	void Warp(Vector2D v);//ベクトルvへ動かす(ステージ編集で用いる)
	void Resize(Vector2D v);//位置ベクトルvを参考に大きさを変更する(ステージ編集で用いる)
	void ChangeShape(std::shared_ptr<Shape> pShape);//当たり判定図形を変更する
	bool JudgePointInsideShape(Vector2D point)const;
	float getTop()const;
	float getBottom()const;
	float getLeft()const;
	float getRight()const;
	Vector2D getPos()const;
	Vector2D getResizeVec()const;
	std::shared_ptr<Shape> getHitJudgeShape()const;
	void WriteOutObjectWholeInfo(std::ofstream &ofs)const;//オブジェクト全体の情報書き出し

	//文字列からBattleObjectを作る関数
	static std::shared_ptr<BattleObject> CreateObject(const std::string &infostr);
};

#endif // !DEF_BATTLEOBJECT_H
