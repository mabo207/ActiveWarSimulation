#include"Unit.h"
#include"DxLib.h"
#include"Circle.h"

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
const float Unit::BattleStatus::maxOP=100.0f;

//------------Unit---------------
const float Unit::unitCircleSize=30.0f;
const float Unit::rivalInpenetratableCircleSize=Unit::unitCircleSize*2.0f;
const float Unit::closeAttackLength=Unit::rivalInpenetratableCircleSize*1.3f;
const float Unit::openAttackLength=Unit::closeAttackLength*2.0f;

Unit::Unit(Vector2D position,int gHandle,Team::Kind team)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(position,unitCircleSize,Shape::Fix::e_static)),gHandle)
	,m_baseStatus(10,100,30,30,20,20,5,4)
	,m_battleStatus(100,0,team)
	,m_rivalInpenetratableCircle(new Circle(position,rivalInpenetratableCircleSize,Shape::Fix::e_static))
{
	//テスト用のコンストラクタ
	m_battleStatus.HP=m_baseStatus.maxHP;
}

Unit::~Unit(){}

void Unit::WriteOutObjectPeculiarInfo(std::ofstream &ofs)const{
	//暫定
	ofs<<"("<<Type::GetStr(m_type)<<")";
}

bool Unit::SetPenetratable(Team::Kind nowPhase){
	return (m_penetratable=(m_battleStatus.team==nowPhase));
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
	SetDrawBlendMode(mode,pal);

	//描画モードを元に戻っている
}

void Unit::VHitProcess(const BattleObject *potherobj){
	//特に何もしない
}

std::shared_ptr<BattleObject> Unit::VCopy()const{
	return std::shared_ptr<BattleObject>(new Unit(m_hitJudgeShape->GetPosition(),m_gHandle,m_battleStatus.team));
}
