#ifndef DEF_UNIT_H
#define DEF_UNIT_H

#include"BattleObject.h"

//ユニットを示す
class Unit:public BattleObject{
	//型・列挙体
public:
	struct Team{
		enum Kind{
			e_player,
			e_enemy,
			END
		};
		static Kind link(int num);
		static unsigned int GetColor(Kind kind);
	};
	struct BaseStatus{
		int lv;
		int maxHP;
		int power;
		int def;
		int mpower;
		int mdef;
		int speed;
		int move;
		BaseStatus(int i_lv,int i_maxHP,int i_power,int i_def,int i_mpower,int i_mdef,int i_speed,int i_move)
			:lv(i_lv),maxHP(i_maxHP),power(i_power),def(i_def),mpower(i_mpower),mdef(i_mdef),speed(i_speed),move(i_move){}
	};
	struct BattleStatus{
		//定数
		static const float maxOP;//オーダーポイントの最大値、全てのユニットで一律
		//変数
		int HP;
		float OP;
		Team::Kind team;
		BattleStatus(int i_HP,float i_OP,Team::Kind i_team)
			:HP(i_HP),OP(i_OP),team(i_team){}
	};

	//定数
protected:
	static const float unitCircleSize;//ユニットそのものの大きさ
	static const float rivalInpenetratableCircleSize;//敵対ユニットの進入不可領域
	static const float closeAttackLength;//短距離攻撃の射程
	static const float openAttackLength;//長距離攻撃の射程

	static const float attackCost;//攻撃のコストを実装(これは武器クラスに突っ込む可能性が高い)

public:

	//変数
protected:
	//基本ステータス
	BaseStatus m_baseStatus;
	//戦闘中ステータス
	BattleStatus m_battleStatus;
	std::shared_ptr<Shape> m_rivalInpenetratableCircle;
	bool m_penetratable;//味方のフェイズ故に敵対ユニット進入不可領域に侵入できるかどうか
	//描画系変数
	//マップ上のキャラグラフィック(BattleObjectにある)
	//ステータス画面のキャラグラフィック

public:
	//武器
	//必殺技
	//アイテム(複数個)

	//関数
protected:
	void WriteOutObjectPeculiarInfo(std::ofstream &ofs)const;//オブジェクト特有の情報を返す

public:
	//コンストラクタ系
	Unit(Vector2D position,int gHandle,Team::Kind team);
	~Unit();
	//演算子オーバーロード

	//getter系
	BaseStatus GetBaseStatus()const{
		return m_baseStatus;
	}
	BattleStatus GetBattleStatus()const{
		return m_battleStatus;
	}
	//通常関数
	bool SetPenetratable(Team::Kind nowPhase);//現在動かしているユニットの所属を見て、侵入可否のフラグを更新する
	void AddOP(float cost);//OPを減らす関数
	void DrawMoveInfo(Vector2D adjust=Vector2D())const;//移動範囲関連の情報を描画する関数(VDraw()と同じようなオーバーロードをする)
	void DrawMoveInfo(Vector2D point,Vector2D adjust)const;//移動範囲関連の情報を描画する関数
	
	//仮想関数・純粋仮想関数のオーバーライド
	const Shape *GetHitJudgeShape()const;//当たり判定図形を取得。
	void Move(Vector2D v);//当たり判定図形の移動。２種類の当たり判定図形を共に移動させる。
	Shape::Fix::Kind SetFix(Shape::Fix::Kind fix)const;//m_hitJudgeShape.m_fixを変更
	void VDraw(Vector2D point,Vector2D adjust)const;//描画処理(任意座標に描画)
	void VHitProcess(const BattleObject *potherobj);//何かに当たった時の処理
	std::shared_ptr<BattleObject> VCopy()const;//同じオブジェクトを複製する(ポインタのみ異なる)
};

#endif // !DEF_UNIT_H

#pragma once
