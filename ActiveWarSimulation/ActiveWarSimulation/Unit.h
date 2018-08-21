#ifndef DEF_UNIT_H
#define DEF_UNIT_H

#include"BattleObject.h"
#include"Weapon.h"
class Weapon;//循環参照を防ぐために宣言のみする

//ユニットを示す
class Unit:public BattleObject{
	//型・列挙体
public:
	struct Profession{
		//兵種
		enum Kind{
			e_lancer,
			e_archer,
			e_armer,
			e_mage,
			e_healer,
			END
		};
		static const std::map<std::string,Profession::Kind> professionMap;
		static Kind link(int num);
		static std::string GetName(Kind kind);
		static Kind GetKind(const std::string &str);
	};
	struct Team{
		enum Kind{
			e_player,
			e_enemy,
			END
		};
		static Kind link(int num);
		static unsigned int GetColor(Kind kind);
		static bool JudgeFriend(Kind team1,Kind team2);//2つのチームが味方関係かどうかを判定する
	};
	struct BaseStatus{
		std::string name;
		Profession::Kind profession;
		int lv;
		int maxHP;
		int power;
		int def;
		int mpower;
		int mdef;
		int speed;
		int move;
		BaseStatus(){}
		BaseStatus(std::string i_name,Profession::Kind i_profession,int i_lv,int i_maxHP,int i_power,int i_def,int i_mpower,int i_mdef,int i_speed,int i_move)
			:name(i_name),profession(i_profession),lv(i_lv),maxHP(i_maxHP),power(i_power),def(i_def),mpower(i_mpower),mdef(i_mdef),speed(i_speed),move(i_move){}
	};
	struct BattleStatus{
		//定数
		static const float maxOP;//オーダーポイントの最大値、全てのユニットで一律
		//変数
		int HP;
		float OP;
		Team::Kind team;
		std::shared_ptr<Weapon> weapon;
		BattleStatus(int i_HP,float i_OP,Team::Kind i_team,std::shared_ptr<Weapon> i_weapon)
			:HP(i_HP),OP(i_OP),team(i_team),weapon(i_weapon){}
	};

	//定数
public:
	static const float unitCircleSize;//ユニットそのものの大きさ
	static const float rivalInpenetratableCircleSize;//敵対ユニットの進入不可領域
	
	static const float attackCost;//攻撃のコストを実装(これは武器クラスに突っ込む可能性が高い)

	static const int hpFontSize;//HPのフォントの大きさ

	static const float reduceStartActionCost;//行動開始時に減らされるコスト

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
	int m_hpFont;//HPのフォント

public:
	//武器
	//必殺技
	//アイテム(複数個)

	//関数
protected:
	void WriteOutObjectPeculiarInfo(std::ofstream &ofs)const;//オブジェクト特有の情報を返す
	float CalculateConsumeOP(float cost)const;//costに対して消費するOPを計算する
	float GetMoveDistance(float vOP)const;//もしvOPだけOPがあったら移動可能な直線距離を返す。定式化しておくとラクになるため
	void DrawMoveInfo(float distance,Vector2D point,Vector2D adjust)const;//移動可能距離がdistanceである時の移動範囲関連の情報を描画する関数

public:
	//コンストラクタ系
	//Unit(Vector2D position,int gHandle,Team::Kind team);
	Unit(BaseStatus baseStatus,std::shared_ptr<Weapon> weapon,Vector2D position,int gHandle,Team::Kind team);
	Unit(const Unit &u);
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
	bool JudgeAttackable(const Unit *pUnit)const;//pUnitに攻撃可能か
	const Shape *GetUnitCircleShape()const;//ユニット自身を示す円を取得
	int AddHP(int pal);//HPを増減させる関数、増減後のHPを返す
	//void AddOP(float cost);//OPを増減させる関数(意味合いが2つあるので分割)
	float ConsumeOPByCost(float cost);//costを指定してOPを消費する関数。消費OP増加などの実装があったらここを弄れば良い。基本的にこの関数を用いてOP操作をする。
	float ConsumeOPVirtualByCost(float cost)const;//もしConsumeOPByCost()をしたらOPはどんな値になるかを返す
	float SetOP(float op);//Unit::OPをopの値にする。移動巻き戻しや行動順制御など、ConsumeOPByCost()を用いることができない時に用いる。
	void DrawMoveInfo(Vector2D adjust=Vector2D())const;//移動範囲関連の情報を描画する関数(VDraw()と同じようなオーバーロードをする)
	void DrawMoveInfo(Vector2D point,Vector2D adjust)const;//移動範囲関連の情報を描画する関数
	void DrawMaxMoveInfo(Vector2D adjust=Vector2D())const;//移動範囲最大の移動範囲関連の情報を描画する関数(VDraw()と同じようなオーバーロードをする)
	void DrawMaxMoveInfo(Vector2D point,Vector2D adjust)const;//移動範囲最大の移動範囲関連の情報を描画する関数
	void DrawHPGage(Vector2D adjust=Vector2D())const;//HPゲージの描画
	void DrawHPGage(Vector2D point,Vector2D adjust)const;//HPゲージの描画
	void DrawFacePic(Vector2D point)const;//ユニットの顔グラフィックを用いたアイコンの描画を行う
	void DrawUnit(Vector2D point,Vector2D adjust,bool infoDrawFlag)const;//マップ上のユニットを描画
	float GetMoveDistance()const;//移動可能な直線距離を返す。定式化しておくとラクになるため
	
	//仮想関数・純粋仮想関数のオーバーライド
	const Shape *GetHitJudgeShape()const;//当たり判定図形を取得。
	void Move(Vector2D v);//当たり判定図形の移動。２種類の当たり判定図形を共に移動させる。
	Shape::Fix::Kind SetFix(Shape::Fix::Kind fix)const;//m_hitJudgeShape.m_fixを変更
	void VDraw(Vector2D point,Vector2D adjust)const;//描画処理(任意座標に描画)
	void VHitProcess(const BattleObject *potherobj);//何かに当たった時の処理
	std::shared_ptr<BattleObject> VCopy()const;//同じオブジェクトを複製する(ポインタのみ異なる)

	//静的関数
public:
	static Unit *CreateMobUnit(std::string name,Profession::Kind profession,int lv,Vector2D position,Team::Kind team);//モブユニットを動的生成する。

};

#endif // !DEF_UNIT_H

#pragma once
