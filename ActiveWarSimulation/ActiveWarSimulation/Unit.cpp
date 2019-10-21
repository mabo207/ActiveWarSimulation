#define _USE_MATH_DEFINES
#include"Unit.h"
#include"DxLib.h"
#include"Circle.h"
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"
#include<math.h>
#include"FilePath.h"
#include<optional>

//------------Unit::Profession---------------
const std::map<std::string,Unit::Profession::Kind> Unit::Profession::professionMap={
	std::pair<std::string,Unit::Profession::Kind>("兵士",Unit::Profession::e_soldier)
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
	return Team::GetColor(kind,255);
}

unsigned int Unit::Team::GetColor(Kind kind,int degree,int red,int green,int blue){
	int r,g,b;
	switch(kind){
	case(e_player):
		r=32;
		g=64;
		b=255;
		break;
	case(e_enemy):
		r=255;
		g=32;
		b=128;
		break;
	default:
		r=128;
		g=128;
		b=128;
		break;
	}
	return DxLib::GetColor((r*degree+red*(255-degree))/255,(g*degree+red*(255-degree))/255,(b*degree+blue*(255-degree))/255);
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

std::string Unit::Team::GetName(Kind team){
	switch(team){
	case(e_player):
		return "player";
	case(e_enemy):
		return "enemy";
	}
	return "";
}

//------------Unit::AIType--------------
Unit::AIType::Kind Unit::AIType::link(int num){
	if(num>=0 && num<END){
		return static_cast<Kind>(num);
	}
	return END;
}

std::string Unit::AIType::GetName(Kind type){
	switch(type){
	case(e_assult):
		return "assult";
	case(e_intercept):
		return "intercept";
	case(e_linkageIntercept):
		return "linkintercept";
	case(e_wait):
		return "wait";
	}
	return "";
}

//------------Unit::BattleStatus---------------
const float Unit::BattleStatus::maxOP=100.0f+Unit::reduceStartActionCost+0.0001f;//キリの良い整数より少しだけ大きくする事でOPをmaxOPまで増やす時にOPが計算誤差で半端な整数にならないようにする。

//------------Unit---------------
const float Unit::unitCircleSize=(float)CommonConstParameter::unitCircleSize;
const float Unit::rivalInpenetratableCircleSize=Unit::unitCircleSize*2.0f;
const float Unit::reduceStartActionCost=50.0f;//定数値なのでヘッダー内初期化と同じ順番になり、ここで初期化してもUnit::BattleStatus::maxOPの初期化より先に処理が行われることになる。
const float Unit::attackCost=50.0f;

const int Unit::hpFontSize=24;

Unit::Unit(BaseStatus baseStatus,std::shared_ptr<Weapon> weapon,Vector2D position,int gHandle,Team::Kind team,AIType::Kind aitype,int aiGroup,std::set<int> aiLinkage)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(position,unitCircleSize,Shape::Fix::e_static)),gHandle)
	,m_baseStatus(baseStatus)
	,m_battleStatus(100,Unit::BattleStatus::maxOP,team,aitype,aiGroup,aiLinkage,weapon)
	,m_rivalInpenetratableCircle(new Circle(position,rivalInpenetratableCircleSize,Shape::Fix::e_static))
	,m_hpFont(LoadFontDataToHandleEX(FilePath::fontDir+"OrderPalFont.dft",2))
{
	//テスト用のコンストラクタ
	m_battleStatus.HP=m_baseStatus.maxHP;
}

Unit::Unit(const Unit &u)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(u.m_hitJudgeShape->GetPosition(),unitCircleSize,Shape::Fix::e_static)),CopyGraph(u.m_gHandle))
	,m_baseStatus(u.m_baseStatus),m_battleStatus(u.m_battleStatus)
	,m_rivalInpenetratableCircle(u.m_rivalInpenetratableCircle->VCopy())
	,m_hpFont(CopyFontToHandle(u.m_hpFont))
{}

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

void Unit::BecomeAssultAI(){
	if(m_battleStatus.aitype==AIType::e_linkageIntercept){
		m_battleStatus.aitype=AIType::e_assult;
	}
}

void Unit::DrawMoveInfo(Vector2D adjust)const{
	DrawMoveInfo(getPos(),adjust);
}

void Unit::DrawMoveInfo(Vector2D point,Vector2D adjust)const{
	const unsigned int inColor=m_battleStatus.team==Team::e_player?GetColor(51,251,255):GetColor(255,150,231);
	const unsigned int outColor=m_battleStatus.team==Team::e_player?GetColor(41,192,192):inColor;
	DrawMoveInfo(GetMoveDistance(),point,adjust,inColor,outColor);
}

void Unit::DrawMoveInfo(float distance,Vector2D point,Vector2D adjust,unsigned int inColor,unsigned int outColor)const{
	RECT rect;
	GetDrawArea(&rect);
	SetDrawArea(0,0,CommonConstParameter::mapSizeX,CommonConstParameter::mapSizeY);
	Vector2D pos=point-adjust;
	//ユニットの移動限界距離をinColor,outColorの円弧で描画
	DrawCircleAA(pos.x,pos.y,distance,100,outColor,FALSE,3.0f);//枠
	DrawCircleAA(pos.x,pos.y,distance,100,inColor,FALSE,1.0f);//枠
	//描画範囲を元に戻す
	SetDrawArea(rect.left,rect.top,rect.right,rect.bottom);
}

void Unit::DrawMaxMoveInfo(Vector2D adjust)const{
	DrawMaxMoveInfo(getPos(),adjust);
}

void Unit::DrawMaxMoveInfo(Vector2D point,Vector2D adjust)const{
	DrawMoveInfo(GetMoveDistance(BattleStatus::maxOP-CalculateConsumeOP(reduceStartActionCost)),point,adjust,Unit::Team::GetColor(m_battleStatus.team,128,255,255,255),Unit::Team::GetColor(m_battleStatus.team,128,220,220,220));//キャラの中の色で塗る
}

void Unit::DrawHPGage(Vector2D adjust)const{
	DrawHPGage(getPos(),adjust);
}

void Unit::DrawHPGage(Vector2D point,Vector2D adjust)const{
	DrawHPGage(point,adjust,1.0f);
}

void Unit::DrawHPGage(Vector2D point,Vector2D adjust,float exRate)const{
	//HPゲージとHPの表示。ゲージは非AAで描画したほうが綺麗に見える
	const Vector2D centerDrawPos=point*exRate+adjust;
	const int gageX=(int)(centerDrawPos.x-unitCircleSize)
		,gageY=(int)(centerDrawPos.y+unitCircleSize)-hpFontSize/2
		,unitCircleSizeInteger=(int)(unitCircleSize)
		,margin=2;
	const int gageMaxLength=(int)((unitCircleSizeInteger-margin)*2*exRate);
	const int gageLength=gageMaxLength*m_battleStatus.HP/m_baseStatus.maxHP;
	//ゲージの色
	unsigned int color;
	//HPの値で色を決める
	const int interval=5;
	if(m_battleStatus.HP<=interval*1){
		color=GetColor(255,64,0);//赤色
	} else if(m_battleStatus.HP<=interval*2){
		color=GetColor(234,106,0);
	} else if(m_battleStatus.HP<=interval*3){
		color=GetColor(213,149,0);
	} else if(m_battleStatus.HP<=interval*4){
		color=GetColor(192,192,0);//黄色
	} else if(m_battleStatus.HP<=interval*5){
		color=GetColor(128,192,0);
	} else if(m_battleStatus.HP<=interval*6){
		color=GetColor(85,192,0);
	} else if(m_battleStatus.HP<=interval*7){
		color=GetColor(32,192,0);//黄緑色
	} else if(m_battleStatus.HP<=interval*8){
		color=GetColor(21,192,85);
	} else if(m_battleStatus.HP<=interval*9){
		color=GetColor(10,192,170);
	} else{
		color=GetColor(0,192,255);//水色
	}
	//ゲージ外側の描画
	DrawBox(gageX,gageY,gageX+unitCircleSizeInteger*2,gageY+hpFontSize,GetColor(0,0,0),TRUE);
	//ゲージ内側の描画
	//DrawBox(gageX+margin,gageY+margin,gageX+margin+gageLength,gageY+hpFontSize-margin,color,TRUE);//単純な描画
	unsigned int leftColor;
	switch(m_battleStatus.team){
	case(Team::e_player):
		leftColor=GetColor(21,106,191);
		break;
	case(Team::e_enemy):
		leftColor=GetColor(191,21,34);
		break;
	default:
		leftColor=GetColor(21,106,191);
		break;
	}
	DrawBoxGradation(gageX+margin,gageY+margin,gageX+margin+gageLength,gageY+hpFontSize-margin,leftColor,color,true);
	//HPの文字列の描画
	DrawStringRightJustifiedToHandle(gageX,gageY,std::to_string(m_battleStatus.HP),GetColor(255,255,255),m_hpFont,GetColor(0,0,0));
}

void Unit::DrawFacePic(Vector2D point)const{
	//円の描画
	const int x=(int)point.x,y=(int)point.y,r=(int)unitCircleSize;
	DrawCircle(x,y,r,Team::GetColor(m_battleStatus.team,128,255,255,255),TRUE);//背景の円の描画(白を50%混ぜる)
	DrawRotaGraph(x,y,1.0,0.0,m_gHandle,TRUE);//グラフィックの描画、暫定でマップ上のユニット絵を使用
	DrawCircle(x,y,r,Team::GetColor(m_battleStatus.team,192,0,0,0),FALSE,3);//背景の枠の描画(黒を25%混ぜる)
}

void Unit::DrawUnit(Vector2D adjust,size_t frame,bool animationFlag,bool infoDrawFlag,bool actionRangeDraw)const{
	DrawUnit(getPos(),adjust,frame,animationFlag,infoDrawFlag,actionRangeDraw);
}

void Unit::DrawUnit(Vector2D point,Vector2D adjust,size_t frame,bool animationFlag,bool infoDrawFlag,bool actionRangeDraw)const{
	DrawUnit(point,adjust,1.0f,frame,animationFlag,infoDrawFlag,actionRangeDraw);
}

void Unit::DrawUnit(Vector2D point,Vector2D adjust,float exRate,size_t frame,bool animationFlag,bool infoDrawFlag,bool actionRangeDraw)const{
	Vector2D pos=point*exRate+adjust;//描画位置
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	//UIの表示領域の設定
	RECT rect;
	GetDrawArea(&rect);
	SetDrawArea(0,0,CommonConstParameter::mapSizeX,CommonConstParameter::mapSizeY);
	//UIの表示
	if(infoDrawFlag){
		//アクションの効果範囲を半透明(弱)で描画
		//ひとまず短射程で描画本来は武器クラスのDraw関数を使うのが望ましい。
		if(actionRangeDraw){
			if(animationFlag){
				//操作キャラの攻撃範囲は枠をつけてユニットチーム色で表現
				const unsigned int color=Team::GetColor(m_battleStatus.team);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA,32);
				DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,color,TRUE);//面
				SetDrawBlendMode(mode,pal);
				DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,color,FALSE);//枠
			} else{
				//操作していないけども攻撃範囲を表示したい時は黄色点滅で表現
				//const unsigned int color=Team::GetColor(m_battleStatus.team);
				const unsigned int color=GetColor(128,128,64);
				const int alpha=(int)((std::sin(frame*0.05)+1.0)*0.5*64);
				//const int alpha=32;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA,alpha);
				DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,color,TRUE);//面
				SetDrawBlendMode(mode,pal);
				//DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,color,FALSE);//枠
			}
		}
		//ユニットの当たり判定図形を描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,32);
		GetHitJudgeShape()->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team),TRUE,1.0f);//面
		SetDrawBlendMode(mode,pal);
		GetHitJudgeShape()->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team),FALSE,1.0f);//枠
		//ユニット自身の当たり判定の描画
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
		m_hitJudgeShape->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team,128,255,255,255),TRUE,1.0f);//面
		m_hitJudgeShape->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team,192,0,0,0),FALSE,3.0f);//枠(黒を25%混ぜる)
		//選択ユニットの当たり判定部分の輝度加算
		if(animationFlag){
			const int addMax=120;
			SetDrawBlendMode(DX_BLENDMODE_ADD,(frame%60)*(60-frame%60)*addMax/900);
			m_hitJudgeShape->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team,128,255,255,255),TRUE,1.0f);//面
			m_hitJudgeShape->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team,192,0,0,0),FALSE,3.0f);//枠(黒を25%混ぜる)
		}
		SetDrawBlendMode(mode,pal);
	}
	//描画範囲を元に戻す
	SetDrawArea(rect.left,rect.top,rect.right,rect.bottom);
	//アニメーションパラメータの設定
	int ux=(int)(pos.x),uy=(int)(pos.y);
	int cx,cy;
	GetGraphSize(m_gHandle,&cx,&cy);
	cx/=2;
	cy/=2;
	double angle=0.0;
	const double defaultRate=1.2*exRate;//デフォの画像拡大率(画像がちゃんと揃ったら係数を1.0にする)
	double exRateX=defaultRate,exRateY=defaultRate;
	if(animationFlag){
		//angle=std::cos(frame/60.0*M_PI)*M_PI/180.0*6.0;
		exRateY=defaultRate-(frame%60)*(60-frame%60)/900.0*0.05;//倍率は1.15~1.2倍、周期は1秒
		uy+=(int)(cy*(defaultRate-exRateY));//画像下を常に揃える
	}
	//ユニットグラフィックを描画
	DrawRotaGraph3(ux,uy,cx,cy,exRateX,exRateY,angle,m_gHandle,TRUE,FALSE);

	//描画モードを元に戻す
	SetDrawBlendMode(mode,pal);
}

float Unit::GetMoveDistance()const{
	//残りOPで移動可能な直線距離を求める。
	//return m_battleStatus.OP/CalculateConsumeOP(1.0f)*m_baseStatus.move;
	return GetMoveDistance(m_battleStatus.OP);
}

float Unit::GetMoveDistance(float vOP)const{
	//残りOPで移動可能な直線距離を求める。
	return vOP/CalculateConsumeOP(1.0f)*m_baseStatus.move;
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
	DrawUnit(point,adjust,0,false,true,false);
}

void Unit::VHitProcess(const BattleObject *potherobj){
	//特に何もしない
}

std::shared_ptr<BattleObject> Unit::VCopy()const{
	return std::shared_ptr<BattleObject>(new Unit(*this));
}

Unit *Unit::CreateMobUnit(const std::string &name,Profession::Kind profession,int lv,const std::string &weaponName,Vector2D position,Team::Kind team,AIType::Kind aitype,int aiGroup,std::set<int> aiLinkage){
	BaseStatus baseStatus;
	std::shared_ptr<Weapon> weapon=Weapon::GetWeapon(weaponName);
	int gHandle=-1;
	switch(profession){
	case(Profession::e_soldier):
		baseStatus=BaseStatus(name,profession,lv,21+(int)(lv*0.8),7+(int)(lv*0.5),6+(int)(lv*0.45),2+(int)(lv*0.1),4+(int)(lv*0.4),6);
		if(team==Team::e_player){
			gHandle=LoadGraphEX(FilePath::graphicDir+"nonfree/soldier_1P.png");
		} else{
			gHandle=LoadGraphEX(FilePath::graphicDir+"nonfree/soldier_2P.png");
		}
		break;
	case(Profession::e_archer):
		baseStatus=BaseStatus(name,profession,lv,19+(int)(lv*0.75),6+(int)(lv*0.45),5+(int)(lv*0.4),2+(int)(lv*0.1),5+(int)(lv*0.45),6);
		if(team==Team::e_player){
			gHandle=LoadGraphEX(FilePath::graphicDir+"nonfree/archer_1P.png");
		} else{
			gHandle=LoadGraphEX(FilePath::graphicDir+"nonfree/archer_2P.png");
		}
		break;
	case(Profession::e_armer):
		baseStatus=BaseStatus(name,profession,lv,25+(int)(lv*0.9),8+(int)(lv*0.6),8+(int)(lv*0.6),0+(int)(lv*0.1),0+(int)(lv*0.2),3);
		if(team==Team::e_player){
			gHandle=LoadGraphEX(FilePath::graphicDir+"nonfree/armerknight_1P.png");
		} else{
			gHandle=LoadGraphEX(FilePath::graphicDir+"nonfree/armerknight_2P.png");
		}
		break;
	case(Profession::e_mage):
		baseStatus=BaseStatus(name,profession,lv,16+(int)(lv*0.6),1+(int)(lv*0.1),2+(int)(lv*0.3),7+(int)(lv*0.6),6+(int)(lv*0.5),4);
		if(team==Team::e_player){
			gHandle=LoadGraphEX(FilePath::graphicDir+"nonfree/mage_1P.png");
		} else{
			gHandle=LoadGraphEX(FilePath::graphicDir+"nonfree/mage_2P.png");
		}
		break;
	case(Profession::e_healer):
		baseStatus=BaseStatus(name,profession,lv,14+(int)(lv*0.5),0+(int)(lv*0.1),2+(int)(lv*0.3),6+(int)(lv*0.5),9+(int)(lv*0.65),6);
		if(team==Team::e_player){
			gHandle=LoadGraphEX(FilePath::graphicDir+"nonfree/healer_1P.png");
		} else{
			gHandle=LoadGraphEX(FilePath::graphicDir+"nonfree/healer_2P.png");
		}
		break;
	}
	return new Unit(baseStatus,weapon,position,gHandle,team,aitype,aiGroup,aiLinkage);
}

Unit *Unit::CreateUnitFromBuilder(StringBuilder &unitdata){
	unitdata.Split(',','(',')');
	//まずモブ用の設定をするか固定ユニット用の設定をするかを判定する
	bool uniqueFlag=false;
	for(const StringBuilder &sb:unitdata.m_vec){
		if(sb.m_vec.size()>=2 && sb.m_vec[0].GetString()=="definition"){
			//設定方法はdefinitionに記載されている
			uniqueFlag=(sb.m_vec[1].GetString()=="unique");
			break;
		}
	}
	//モブか固定かで分岐
	if(uniqueFlag){
		//固定ユニット(未実装)

	} else{
		//モブ
		//各値を宣言。設定したかどうかをpairのsecondに格納
		//こちらは設定必須のもの
		std::optional<std::string> name;
		std::optional<Vector2D> pos;
		std::optional<int> lv;
		std::optional<Unit::Profession::Kind> prof;
		std::optional<std::string> weaponName;
		std::optional<Unit::Team::Kind> team;
		std::optional<Unit::AIType::Kind> aitype;
		std::optional<int> aiGroup;
		std::set<int> aiLinkage;
		//こっちは設定任意のもの
		std::optional<int> initHP;
		std::optional<float> initOP;
		//各値の読み取り
		for(const StringBuilder &sb:unitdata.m_vec){
			try{
				if(!sb.m_vec.empty()){
					//先頭文字列があることを保障
					if(sb.m_vec[0].GetString()=="name" && sb.m_vec.size()>=2){
						name=sb.m_vec[1].GetString();
					} else if(sb.m_vec[0].GetString()=="profession" && sb.m_vec.size()>=2){
						prof=Unit::Profession::link(std::stoi(sb.m_vec[1].GetString().c_str()));
					} else if(sb.m_vec[0].GetString()=="lv" && sb.m_vec.size()>=2){
						lv=std::stoi(sb.m_vec[1].GetString().c_str());
					} else if(sb.m_vec[0].GetString()=="weapon" && sb.m_vec.size()>=2){
						weaponName=sb.m_vec[1].GetString();
					} else if(sb.m_vec[0].GetString()=="pos" && sb.m_vec.size()>=3){
						pos=Vector2D((float)std::stoi(sb.m_vec[1].GetString().c_str()),(float)std::stoi(sb.m_vec[2].GetString().c_str()));
					} else if(sb.m_vec[0].GetString()=="team" && sb.m_vec.size()>=2){
						team=Unit::Team::link(std::stoi(sb.m_vec[1].GetString().c_str()));
					} else if(sb.m_vec[0].GetString()=="ai" && sb.m_vec.size()>=3){
						//aiのコンマ列に2つ以上の値が存在しても良いので、複数変数を受け取れるようにできている
						//1つ目はAIの種類、2つ目は連動型AI用のグループ値、3つ目以降は自由(現状すべての値をaiLinkageに突っ込むようにしている)
						aitype=Unit::AIType::link(std::stoi(sb.m_vec[1].GetString().c_str()));
						aiGroup=std::stoi(sb.m_vec[2].GetString().c_str());
						for(size_t i=3,size=sb.m_vec.size();i<size;i++){
							aiLinkage.insert(std::stoi(sb.m_vec[i].GetString().c_str()));
						}
					} else if(sb.m_vec[0].GetString()=="initHP" && sb.m_vec.size()>=2){
						initHP=std::stoi(sb.m_vec[1].GetString().c_str());
					} else if(sb.m_vec[0].GetString()=="initOP" && sb.m_vec.size()>=2){
						initOP=(float)(std::stoi(sb.m_vec[1].GetString().c_str()));
					}
				}
			} catch(const std::invalid_argument &){
				//数値じゃないものを検出した場合
			} catch(const std::out_of_range &){
				//表現の範囲外の数値を検出した場合
			}
		}
		//各値からユニットを格納
		if(name && prof && lv && weaponName && pos && team && aiGroup && aitype){
			//設定必須である項目が設定されているか
			Unit * const pu=Unit::CreateMobUnit(name.value(),prof.value(),lv.value(),weaponName.value(),pos.value(),team.value(),aitype.value(),aiGroup.value(),aiLinkage);
			//設定任意である項目の設定
			if(initHP && initHP.value()>0 && initHP.value()<pu->GetBattleStatus().HP){
				//HPが0以下だったり最大HPより大きくなったりしないようにする
				pu->AddHP(initHP.value()-pu->GetBattleStatus().HP);
			}
			if(initOP && initOP.value()<Unit::BattleStatus::maxOP){
				//OPはmaxOPを上回らないようにする。0以下になるのは問題ない
				pu->SetOP(initOP.value());
			}
			return pu;
		}
	}
	return nullptr;
}
