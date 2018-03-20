#include"Unit.h"
#include"DxLib.h"
#include"Circle.h"
#include"GraphicControl.h"
#include"ToolsLib.h"

//------------Unit::Team---------------
Unit::Team::Kind Unit::Team::link(int num){
	if(num>=0 && num<END){
		return static_cast<Kind>(num);
	}
	return END;
}

unsigned int Unit::Team::GetColor(Kind kind){
	switch(kind){
	case(e_player):
		return DxLib::GetColor(64,64,255);
	case(e_enemy):
		return DxLib::GetColor(255,64,64);
	}
	return DxLib::GetColor(128,128,128);
}

//------------Unit::BattleStatus---------------
const float Unit::BattleStatus::maxOP=100.0f+0.0001f;//キリの良い整数より少しだけ大きくする事でOPをmaxOPまで増やす時にOPが計算誤差で半端な整数にならないようにする。

//------------Unit---------------
const float Unit::unitCircleSize=30.0f;
const float Unit::rivalInpenetratableCircleSize=Unit::unitCircleSize*2.0f;
const float Unit::closeAttackLength=Unit::rivalInpenetratableCircleSize*1.3f;
const float Unit::openAttackLength=Unit::closeAttackLength*2.0f;

const float Unit::attackCost=50.0f;

const int Unit::hpFontSize=10;

Unit::Unit(Vector2D position,int gHandle,Team::Kind team)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(position,unitCircleSize,Shape::Fix::e_static)),gHandle)
	,m_baseStatus(10,100,30,12,20,10,5,4)
	,m_battleStatus(100,0,team)
	,m_rivalInpenetratableCircle(new Circle(position,rivalInpenetratableCircleSize,Shape::Fix::e_static))
	,m_hpFont(CreateFontToHandleEX("メイリオ",hpFontSize,1,DX_FONTTYPE_EDGE))
{
	//テスト用のコンストラクタ
	m_battleStatus.HP=m_baseStatus.maxHP;
}

Unit::~Unit(){
	//フォントの解放
	DeleteFontToHandleEX(m_hpFont);
}

void Unit::WriteOutObjectPeculiarInfo(std::ofstream &ofs)const{
	//暫定
	ofs<<"("<<Type::GetStr(m_type)<<")";
}

bool Unit::SetPenetratable(Team::Kind nowPhase){
	return (m_penetratable=(m_battleStatus.team==nowPhase));
}

bool Unit::JudgeAttackable(const Unit *pUnit)const{
	if(GetBattleStatus().team==pUnit->GetBattleStatus().team){
		//同じチームなら攻撃できない
		return false;
	}
	//攻撃の射程と位置関係による条件
	std::shared_ptr<Shape> pWeapon(new Circle(getPos(),closeAttackLength,Shape::Fix::e_dynamic));
	if(pWeapon->CalculatePushVec(pUnit->GetUnitCircleShape())==Vector2D()){
		//攻撃範囲に敵ユニット本体がいなければ
		return false;
	}

	return true;
}

const Shape *Unit::GetUnitCircleShape()const{
	return m_hitJudgeShape.get();
}

int Unit::AddHP(int pal){
	m_battleStatus.HP+=pal;
	if(m_battleStatus.HP>m_baseStatus.maxHP){
		m_battleStatus.HP=m_baseStatus.maxHP;
	} else if(m_battleStatus.HP<0){
		m_battleStatus.HP=0;
	}
	return m_battleStatus.HP;
}

void Unit::AddOP(float cost){
	m_battleStatus.OP+=cost;
}

float Unit::CalculateAddOPNormalAttack()const{
	//武器によって決まるが、ひとまず決め打ちの値を返す
	return -attackCost;
}

void Unit::DrawMoveInfo(Vector2D adjust)const{
	DrawMoveInfo(getPos(),adjust);
}

void Unit::DrawMoveInfo(Vector2D point,Vector2D adjust)const{
	Vector2D pos=point-adjust;
	//ユニットの移動限界距離を緑を描画
	DrawCircleAA(pos.x,pos.y,m_battleStatus.OP*m_baseStatus.move,100,DxLib::GetColor(0,255,0),FALSE);//枠
	//ユニットの攻撃可能な移動限界距離を水色で描画(攻撃可能な場合のみ)
	if(m_battleStatus.OP>Unit::attackCost){
		DrawCircleAA(pos.x,pos.y,(m_battleStatus.OP-Unit::attackCost)*m_baseStatus.move,100,DxLib::GetColor(0,255,255),FALSE);//枠
	}

}

const Shape *Unit::GetHitJudgeShape()const{
	if(m_penetratable){
		//味方の行動フェイズならば、ユニット自身の当たり判定図形を返す
		return m_hitJudgeShape.get();
	} else{
		//敵対ユニットの行動フェイズなら、進入不可領域の当たり判定図形を返す
		return m_rivalInpenetratableCircle.get();
	}
}

void Unit::Move(Vector2D v){
	//2つの図形を動かす
	m_hitJudgeShape->Move(v);
	m_rivalInpenetratableCircle->Move(v);
}

Shape::Fix::Kind Unit::SetFix(Shape::Fix::Kind fix)const{
	m_hitJudgeShape->m_fix=fix;
	return (m_rivalInpenetratableCircle->m_fix=fix);
}

void Unit::VDraw(Vector2D point,Vector2D adjust)const{
	Vector2D pos=point-adjust;
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	//アクションの効果範囲を半透明(弱)で描画
	//ひとまず短射程で描画本来は武器クラスのDraw関数を使うのが望ましい。
	if(GetFix()==Shape::Fix::e_dynamic){
		//dynamicなキャラのみアクション範囲を表示。恐らく移動しているキャラのみ
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
		DrawCircleAA(pos.x,pos.y,closeAttackLength,100,Team::GetColor(m_battleStatus.team),TRUE);//面
		SetDrawBlendMode(mode,pal);
		DrawCircleAA(pos.x,pos.y,closeAttackLength,100,Team::GetColor(m_battleStatus.team),FALSE);//枠
	}
	//ユニットの当たり判定図形を描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
	GetHitJudgeShape()->Draw(adjust,Team::GetColor(m_battleStatus.team),TRUE);//面
	SetDrawBlendMode(mode,pal);
	GetHitJudgeShape()->Draw(adjust,Team::GetColor(m_battleStatus.team),FALSE);//枠
	//ユニット自身の当たり判定の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
	m_hitJudgeShape->Draw(adjust,Team::GetColor(m_battleStatus.team),TRUE);//面
	SetDrawBlendMode(mode,pal);
	m_hitJudgeShape->Draw(adjust,Team::GetColor(m_battleStatus.team),FALSE);//面
	//ユニットグラフィックを描画

	//HPゲージとHPの表示。ゲージは非AAで描画したほうが綺麗に見える
	const int gageX=(int)(getPos().x-unitCircleSize),gageY=(int)(getPos().y+unitCircleSize)-hpFontSize/2;
	DrawBox(gageX,gageY,gageX+(int)(unitCircleSize*2),gageY+hpFontSize,GetColor(0,0,0),TRUE);//ゲージ外側
	DrawBox(gageX+1,gageY+1,gageX+(int)(unitCircleSize)*2*m_battleStatus.HP/m_baseStatus.maxHP-1,gageY+hpFontSize-1,GetColor(0,128,255),TRUE);//ゲージ内側
	DrawStringRightJustifiedToHandle(gageX,gageY,std::to_string(m_battleStatus.HP),GetColor(255,255,255),m_hpFont,GetColor(0,0,0));//HPの文字列
	//描画モードを元に戻す
	SetDrawBlendMode(mode,pal);
}

void Unit::VHitProcess(const BattleObject *potherobj){
	//特に何もしない
}

std::shared_ptr<BattleObject> Unit::VCopy()const{
	return std::shared_ptr<BattleObject>(new Unit(m_hitJudgeShape->GetPosition(),m_gHandle,m_battleStatus.team));
}
