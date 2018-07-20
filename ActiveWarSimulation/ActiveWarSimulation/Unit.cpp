#include"Unit.h"
#include"DxLib.h"
#include"Circle.h"
#include"GraphicControl.h"
#include"ToolsLib.h"

//------------Unit::Profession---------------
const std::map<std::string,Unit::Profession::Kind> Unit::Profession::professionMap={
	std::pair<std::string,Unit::Profession::Kind>("槍術士",Unit::Profession::e_lancer)
	,std::pair<std::string,Unit::Profession::Kind>("射手",Unit::Profession::e_archer)
	,std::pair<std::string,Unit::Profession::Kind>("重装兵",Unit::Profession::e_armer)
	,std::pair<std::string,Unit::Profession::Kind>("魔道士",Unit::Profession::e_mage)
	,std::pair<std::string,Unit::Profession::Kind>("衛生兵",Unit::Profession::e_healer)
};

Unit::Profession::Kind Unit::Profession::link(int num){
	if(num>=0 && num<END){
		return static_cast<Kind>(num);
	}
	return END;
}

std::string Unit::Profession::GetName(Kind kind){
	for(std::map<std::string,Unit::Profession::Kind>::const_iterator it=professionMap.begin(),ite=professionMap.end();it!=ite;it++){
		if(it->second==kind){
			return it->first;
		}
	}
	return "";//見つからなかったら空文字列を返す
}

Unit::Profession::Kind Unit::Profession::GetKind(const std::string &str){
	std::map<std::string,Unit::Profession::Kind>::const_iterator it=professionMap.find(str);
	if(it==professionMap.end()){
		//見つからなかった場合
		return END;
	} else{
		return it->second;
	}
}

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

bool Unit::Team::JudgeFriend(Kind team1,Kind team2){
	if(team1==team2){
		//チーム名が同じなら味方
		return true;
	}
	//その他味方である組み合わせを列挙


	//いずれでもないなら敵
	return false;
}

//------------Unit::BattleStatus---------------
const float Unit::BattleStatus::maxOP=100.0f+Unit::reduceStartActionCost+0.0001f;//キリの良い整数より少しだけ大きくする事でOPをmaxOPまで増やす時にOPが計算誤差で半端な整数にならないようにする。

//------------Unit---------------
const float Unit::unitCircleSize=45.0f;
const float Unit::rivalInpenetratableCircleSize=Unit::unitCircleSize*2.0f;
const float Unit::reduceStartActionCost=50.0f;

const float Unit::attackCost=50.0f;

const int Unit::hpFontSize=20;

Unit::Unit(Vector2D position,int gHandle,Team::Kind team)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(position,unitCircleSize,Shape::Fix::e_static)),gHandle)
	,m_baseStatus("もぶ",Profession::e_lancer,2,20,5,3,2,3,5,4)
	,m_battleStatus(20,0,team,Weapon::GetWeapon("鉄の槍"))
	,m_rivalInpenetratableCircle(new Circle(position,rivalInpenetratableCircleSize,Shape::Fix::e_static))
	,m_hpFont(CreateFontToHandleEX("メイリオ",hpFontSize,1,DX_FONTTYPE_EDGE))
{
	//テスト用のコンストラクタ
	m_battleStatus.HP=m_baseStatus.maxHP;
}

Unit::Unit(BaseStatus baseStatus,std::shared_ptr<Weapon> weapon,Vector2D position,int gHandle,Team::Kind team)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(position,unitCircleSize,Shape::Fix::e_static)),gHandle)
	,m_baseStatus(baseStatus),m_battleStatus(100,Unit::BattleStatus::maxOP,team,weapon)
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

float Unit::CalculateConsumeOP(float cost)const{
	return cost;
}

bool Unit::SetPenetratable(Team::Kind nowPhase){
	return (m_penetratable=Team::JudgeFriend(m_battleStatus.team,nowPhase));
}

bool Unit::JudgeAttackable(const Unit *pUnit)const{
	if(m_battleStatus.weapon.get()==nullptr || !m_battleStatus.weapon->JudgeWeild(this,pUnit)){
		//各武器に設定されているチーム条件を満たさない場合は攻撃できない
		return false;
	}
	//攻撃の射程と位置関係による条件
	std::shared_ptr<Shape> pWeapon(new Circle(getPos(),m_battleStatus.weapon->GetLength(),Shape::Fix::e_dynamic));
	if(!pWeapon->JudgeInShape(pUnit->GetUnitCircleShape())){
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

/*
void Unit::AddOP(float cost){
	m_battleStatus.OP+=cost;
}
//*/

float Unit::ConsumeOPByCost(float cost){
	//ひとまずこれで。costに倍率をかけたりする。
	return (m_battleStatus.OP=ConsumeOPVirtualByCost(cost));
}

float Unit::ConsumeOPVirtualByCost(float cost)const{
	return m_battleStatus.OP-CalculateConsumeOP(cost);
}

float Unit::SetOP(float op){
	return m_battleStatus.OP=op;
}

void Unit::DrawMoveInfo(Vector2D adjust)const{
	DrawMoveInfo(getPos(),adjust);
}

void Unit::DrawMoveInfo(Vector2D point,Vector2D adjust)const{
	Vector2D pos=point-adjust;
	//ユニットの移動限界距離を緑を描画
	DrawCircleAA(pos.x,pos.y,GetMoveDistance(),100,DxLib::GetColor(0,255,0),FALSE);//枠
	//ユニットの攻撃可能な移動限界距離を水色で描画(攻撃可能な場合のみ)
	if(m_battleStatus.OP>Unit::attackCost){
		DrawCircleAA(pos.x,pos.y,(m_battleStatus.OP-Unit::attackCost)*m_baseStatus.move,100,DxLib::GetColor(0,255,255),FALSE);//枠
	}

}

void Unit::DrawHPGage(Vector2D adjust)const{
	DrawHPGage(getPos(),adjust);
}

void Unit::DrawHPGage(Vector2D point,Vector2D adjust)const{
	//HPゲージとHPの表示。ゲージは非AAで描画したほうが綺麗に見える
	const int gageX=(int)(getPos().x-unitCircleSize),gageY=(int)(getPos().y+unitCircleSize)-hpFontSize/2,unitCircleSizeInteger=(int)(unitCircleSize),margin=2;
	const int gageMaxLength=(unitCircleSizeInteger-margin)*2;
	const int gageLength=gageMaxLength*m_battleStatus.HP/m_baseStatus.maxHP;
	unsigned int color;//ゲージの色
	if(gageLength>gageMaxLength*3/4){
		//HPが全体の3/4以上なら水色
		color=GetColor(0,196,255);
	} else if(gageLength>gageMaxLength*2/4){
		//HPが全体の1/2以上なら黄緑色
		color=GetColor(32,196,0);
	} else if(gageLength>gageMaxLength*1/4){
		//HPが全体の1/4以上なら黄色
		color=GetColor(196,196,0);
	} else{
		//HPが全体の1/4以下なら赤色
		color=GetColor(255,64,0);
	}
	DrawBox(gageX,gageY,gageX+unitCircleSizeInteger*2,gageY+hpFontSize,GetColor(0,0,0),TRUE);//ゲージ外側
	DrawBox(gageX+margin,gageY+margin,gageX+margin+gageLength,gageY+hpFontSize-margin,color,TRUE);//ゲージ内側
	DrawStringRightJustifiedToHandle(gageX,gageY,std::to_string(m_battleStatus.HP),GetColor(255,255,255),m_hpFont,GetColor(0,0,0));//HPの文字列

}

void Unit::DrawFacePic(Vector2D point)const{
	//円の描画
	const int x=(int)point.x,y=(int)point.y,r=(int)unitCircleSize;
	DrawCircle(x,y,r,Team::GetColor(m_battleStatus.team),TRUE);//背景の円の描画
	DrawRotaGraph(x,y,1.0,0.0,m_gHandle,TRUE);//グラフィックの描画、暫定でマップ上のユニット絵を使用
	DrawCircle(x,y,r,GetColor(255,255,255),FALSE,3);//背景の枠の描画
}

void Unit::DrawUnit(Vector2D point,Vector2D adjust,bool infoDrawFlag)const{
	Vector2D pos=point-adjust;
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	if(infoDrawFlag){
		//アクションの効果範囲を半透明(弱)で描画
		//ひとまず短射程で描画本来は武器クラスのDraw関数を使うのが望ましい。
		if(GetFix()==Shape::Fix::e_dynamic){
			//dynamicなキャラのみアクション範囲を表示。恐らく移動しているキャラのみ
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,32);
			DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,Team::GetColor(m_battleStatus.team),TRUE);//面
			SetDrawBlendMode(mode,pal);
			DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,Team::GetColor(m_battleStatus.team),FALSE);//枠
		}
		//ユニットの当たり判定図形を描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,32);
		GetHitJudgeShape()->Draw(pos,adjust,Team::GetColor(m_battleStatus.team),TRUE);//面
		SetDrawBlendMode(mode,pal);
		GetHitJudgeShape()->Draw(pos,adjust,Team::GetColor(m_battleStatus.team),FALSE);//枠
		//ユニット自身の当たり判定の描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
		m_hitJudgeShape->Draw(pos,adjust,Team::GetColor(m_battleStatus.team),TRUE);//面
		SetDrawBlendMode(mode,pal);
		m_hitJudgeShape->Draw(pos,adjust,Team::GetColor(m_battleStatus.team),FALSE);//面
	}
	//ユニットグラフィックを描画
	DrawRotaGraph((int)(pos.x),(int)(pos.y),1.0,0.0,m_gHandle,TRUE,FALSE);
	//描画モードを元に戻す
	SetDrawBlendMode(mode,pal);
}

float Unit::GetMoveDistance()const{
	//残りOPで移動可能な直線距離を求める。
	return m_battleStatus.OP/CalculateConsumeOP(1.0f)*m_baseStatus.move;
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
	DrawUnit(point,adjust,true);
}

void Unit::VHitProcess(const BattleObject *potherobj){
	//特に何もしない
}

std::shared_ptr<BattleObject> Unit::VCopy()const{
	return std::shared_ptr<BattleObject>(new Unit(m_hitJudgeShape->GetPosition(),m_gHandle,m_battleStatus.team));
}

Unit *Unit::CreateMobUnit(std::string name,Profession::Kind profession,int lv,Vector2D position,Team::Kind team){
	BaseStatus baseStatus;
	std::shared_ptr<Weapon> weapon;
	int gHandle=-1;
	switch(profession){
	case(Profession::e_lancer):
		baseStatus=BaseStatus(name,profession,lv,20+(int)(lv*0.8),5+(int)(lv*0.5),3+(int)(lv*0.3),2+(int)(lv*0.1),3+(int)(lv*0.3),5+(int)(lv*0.5),6);
		weapon=Weapon::GetWeapon("鉄の槍");
		gHandle=LoadGraphEX("Graphic/soldier.png");
		break;
	case(Profession::e_archer):
		baseStatus=BaseStatus(name,profession,lv,18+(int)(lv*0.75),4+(int)(lv*0.45),3+(int)(lv*0.3),2+(int)(lv*0.1),3+(int)(lv*0.3),3+(int)(lv*0.3),6);
		weapon=Weapon::GetWeapon("鉄の弓");
		gHandle=LoadGraphEX("Graphic/archer.png");
		break;
	case(Profession::e_armer):
		baseStatus=BaseStatus(name,profession,lv,25+(int)(lv*0.9),6+(int)(lv*0.6),6+(int)(lv*0.6),0+(int)(lv*0.1),0+(int)(lv*0.1),1+(int)(lv*0.2),3);
		weapon=Weapon::GetWeapon("鉄の槍");
		gHandle=LoadGraphEX("Graphic/armerknight.png");
		break;
	case(Profession::e_mage):
		baseStatus=BaseStatus(name,profession,lv,16+(int)(lv*0.6),1+(int)(lv*0.1),1+(int)(lv*0.2),6+(int)(lv*0.6),5+(int)(lv*0.4),5+(int)(lv*0.5),4);
		weapon=Weapon::GetWeapon("ファイアーの書");
		gHandle=LoadGraphEX("Graphic/mage.png");
		break;
	case(Profession::e_healer):
		baseStatus=BaseStatus(name,profession,lv,13+(int)(lv*0.5),0+(int)(lv*0.1),1+(int)(lv*0.2),5+(int)(lv*0.55),7+(int)(lv*0.5),4+(int)(lv*0.4),6);
		weapon=Weapon::GetWeapon("ヒールの杖");
		gHandle=LoadGraphEX("Graphic/healer.png");
		break;
	}
	return new Unit(baseStatus,weapon,position,gHandle,team);
}
