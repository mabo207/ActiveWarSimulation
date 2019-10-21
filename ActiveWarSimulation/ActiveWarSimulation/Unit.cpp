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
	std::pair<std::string,Unit::Profession::Kind>("���m",Unit::Profession::e_soldier)
	,std::pair<std::string,Unit::Profession::Kind>("�ˎ�",Unit::Profession::e_archer)
	,std::pair<std::string,Unit::Profession::Kind>("�d����",Unit::Profession::e_armer)
	,std::pair<std::string,Unit::Profession::Kind>("�����m",Unit::Profession::e_mage)
	,std::pair<std::string,Unit::Profession::Kind>("�q����",Unit::Profession::e_healer)
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
	return "";//������Ȃ�������󕶎����Ԃ�
}

Unit::Profession::Kind Unit::Profession::GetKind(const std::string &str){
	std::map<std::string,Unit::Profession::Kind>::const_iterator it=professionMap.find(str);
	if(it==professionMap.end()){
		//������Ȃ������ꍇ
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
		//�`�[�����������Ȃ疡��
		return true;
	}
	//���̑������ł���g�ݍ��킹���


	//������ł��Ȃ��Ȃ�G
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
const float Unit::BattleStatus::maxOP=100.0f+Unit::reduceStartActionCost+0.0001f;//�L���̗ǂ�������菭�������傫�����鎖��OP��maxOP�܂ő��₷����OP���v�Z�덷�Ŕ��[�Ȑ����ɂȂ�Ȃ��悤�ɂ���B

//------------Unit---------------
const float Unit::unitCircleSize=(float)CommonConstParameter::unitCircleSize;
const float Unit::rivalInpenetratableCircleSize=Unit::unitCircleSize*2.0f;
const float Unit::reduceStartActionCost=50.0f;//�萔�l�Ȃ̂Ńw�b�_�[���������Ɠ������ԂɂȂ�A�����ŏ��������Ă�Unit::BattleStatus::maxOP�̏���������ɏ������s���邱�ƂɂȂ�B
const float Unit::attackCost=50.0f;

const int Unit::hpFontSize=24;

Unit::Unit(BaseStatus baseStatus,std::shared_ptr<Weapon> weapon,Vector2D position,int gHandle,Team::Kind team,AIType::Kind aitype,int aiGroup,std::set<int> aiLinkage)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(position,unitCircleSize,Shape::Fix::e_static)),gHandle)
	,m_baseStatus(baseStatus)
	,m_battleStatus(100,Unit::BattleStatus::maxOP,team,aitype,aiGroup,aiLinkage,weapon)
	,m_rivalInpenetratableCircle(new Circle(position,rivalInpenetratableCircleSize,Shape::Fix::e_static))
	,m_hpFont(LoadFontDataToHandleEX(FilePath::fontDir+"OrderPalFont.dft",2))
{
	//�e�X�g�p�̃R���X�g���N�^
	m_battleStatus.HP=m_baseStatus.maxHP;
}

Unit::Unit(const Unit &u)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(u.m_hitJudgeShape->GetPosition(),unitCircleSize,Shape::Fix::e_static)),CopyGraph(u.m_gHandle))
	,m_baseStatus(u.m_baseStatus),m_battleStatus(u.m_battleStatus)
	,m_rivalInpenetratableCircle(u.m_rivalInpenetratableCircle->VCopy())
	,m_hpFont(CopyFontToHandle(u.m_hpFont))
{}

Unit::~Unit(){
	//�t�H���g�̉��
	DeleteFontToHandleEX(m_hpFont);
}

void Unit::WriteOutObjectPeculiarInfo(std::ofstream &ofs)const{
	//�b��
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
		//�e����ɐݒ肳��Ă���`�[�������𖞂����Ȃ��ꍇ�͍U���ł��Ȃ�
		return false;
	}
	//�U���̎˒��ƈʒu�֌W�ɂ�����
	std::shared_ptr<Shape> pWeapon(new Circle(getPos(),m_battleStatus.weapon->GetLength(),Shape::Fix::e_dynamic));
	if(!pWeapon->JudgeInShape(pUnit->GetUnitCircleShape())){
		//�U���͈͂ɓG���j�b�g�{�̂����Ȃ����
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
	//�ЂƂ܂�����ŁBcost�ɔ{�����������肷��B
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
	//���j�b�g�̈ړ����E������inColor,outColor�̉~�ʂŕ`��
	DrawCircleAA(pos.x,pos.y,distance,100,outColor,FALSE,3.0f);//�g
	DrawCircleAA(pos.x,pos.y,distance,100,inColor,FALSE,1.0f);//�g
	//�`��͈͂����ɖ߂�
	SetDrawArea(rect.left,rect.top,rect.right,rect.bottom);
}

void Unit::DrawMaxMoveInfo(Vector2D adjust)const{
	DrawMaxMoveInfo(getPos(),adjust);
}

void Unit::DrawMaxMoveInfo(Vector2D point,Vector2D adjust)const{
	DrawMoveInfo(GetMoveDistance(BattleStatus::maxOP-CalculateConsumeOP(reduceStartActionCost)),point,adjust,Unit::Team::GetColor(m_battleStatus.team,128,255,255,255),Unit::Team::GetColor(m_battleStatus.team,128,220,220,220));//�L�����̒��̐F�œh��
}

void Unit::DrawHPGage(Vector2D adjust)const{
	DrawHPGage(getPos(),adjust);
}

void Unit::DrawHPGage(Vector2D point,Vector2D adjust)const{
	DrawHPGage(point,adjust,1.0f);
}

void Unit::DrawHPGage(Vector2D point,Vector2D adjust,float exRate)const{
	//HP�Q�[�W��HP�̕\���B�Q�[�W�͔�AA�ŕ`�悵���ق����Y��Ɍ�����
	const Vector2D centerDrawPos=point*exRate+adjust;
	const int gageX=(int)(centerDrawPos.x-unitCircleSize)
		,gageY=(int)(centerDrawPos.y+unitCircleSize)-hpFontSize/2
		,unitCircleSizeInteger=(int)(unitCircleSize)
		,margin=2;
	const int gageMaxLength=(int)((unitCircleSizeInteger-margin)*2*exRate);
	const int gageLength=gageMaxLength*m_battleStatus.HP/m_baseStatus.maxHP;
	//�Q�[�W�̐F
	unsigned int color;
	//HP�̒l�ŐF�����߂�
	const int interval=5;
	if(m_battleStatus.HP<=interval*1){
		color=GetColor(255,64,0);//�ԐF
	} else if(m_battleStatus.HP<=interval*2){
		color=GetColor(234,106,0);
	} else if(m_battleStatus.HP<=interval*3){
		color=GetColor(213,149,0);
	} else if(m_battleStatus.HP<=interval*4){
		color=GetColor(192,192,0);//���F
	} else if(m_battleStatus.HP<=interval*5){
		color=GetColor(128,192,0);
	} else if(m_battleStatus.HP<=interval*6){
		color=GetColor(85,192,0);
	} else if(m_battleStatus.HP<=interval*7){
		color=GetColor(32,192,0);//���ΐF
	} else if(m_battleStatus.HP<=interval*8){
		color=GetColor(21,192,85);
	} else if(m_battleStatus.HP<=interval*9){
		color=GetColor(10,192,170);
	} else{
		color=GetColor(0,192,255);//���F
	}
	//�Q�[�W�O���̕`��
	DrawBox(gageX,gageY,gageX+unitCircleSizeInteger*2,gageY+hpFontSize,GetColor(0,0,0),TRUE);
	//�Q�[�W�����̕`��
	//DrawBox(gageX+margin,gageY+margin,gageX+margin+gageLength,gageY+hpFontSize-margin,color,TRUE);//�P���ȕ`��
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
	//HP�̕�����̕`��
	DrawStringRightJustifiedToHandle(gageX,gageY,std::to_string(m_battleStatus.HP),GetColor(255,255,255),m_hpFont,GetColor(0,0,0));
}

void Unit::DrawFacePic(Vector2D point)const{
	//�~�̕`��
	const int x=(int)point.x,y=(int)point.y,r=(int)unitCircleSize;
	DrawCircle(x,y,r,Team::GetColor(m_battleStatus.team,128,255,255,255),TRUE);//�w�i�̉~�̕`��(����50%������)
	DrawRotaGraph(x,y,1.0,0.0,m_gHandle,TRUE);//�O���t�B�b�N�̕`��A�b��Ń}�b�v��̃��j�b�g�G���g�p
	DrawCircle(x,y,r,Team::GetColor(m_battleStatus.team,192,0,0,0),FALSE,3);//�w�i�̘g�̕`��(����25%������)
}

void Unit::DrawUnit(Vector2D adjust,size_t frame,bool animationFlag,bool infoDrawFlag,bool actionRangeDraw)const{
	DrawUnit(getPos(),adjust,frame,animationFlag,infoDrawFlag,actionRangeDraw);
}

void Unit::DrawUnit(Vector2D point,Vector2D adjust,size_t frame,bool animationFlag,bool infoDrawFlag,bool actionRangeDraw)const{
	DrawUnit(point,adjust,1.0f,frame,animationFlag,infoDrawFlag,actionRangeDraw);
}

void Unit::DrawUnit(Vector2D point,Vector2D adjust,float exRate,size_t frame,bool animationFlag,bool infoDrawFlag,bool actionRangeDraw)const{
	Vector2D pos=point*exRate+adjust;//�`��ʒu
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	//UI�̕\���̈�̐ݒ�
	RECT rect;
	GetDrawArea(&rect);
	SetDrawArea(0,0,CommonConstParameter::mapSizeX,CommonConstParameter::mapSizeY);
	//UI�̕\��
	if(infoDrawFlag){
		//�A�N�V�����̌��ʔ͈͂𔼓���(��)�ŕ`��
		//�ЂƂ܂��Z�˒��ŕ`��{���͕���N���X��Draw�֐����g���̂��]�܂����B
		if(actionRangeDraw){
			if(animationFlag){
				//����L�����̍U���͈͂͘g�����ă��j�b�g�`�[���F�ŕ\��
				const unsigned int color=Team::GetColor(m_battleStatus.team);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA,32);
				DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,color,TRUE);//��
				SetDrawBlendMode(mode,pal);
				DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,color,FALSE);//�g
			} else{
				//���삵�Ă��Ȃ����ǂ��U���͈͂�\�����������͉��F�_�łŕ\��
				//const unsigned int color=Team::GetColor(m_battleStatus.team);
				const unsigned int color=GetColor(128,128,64);
				const int alpha=(int)((std::sin(frame*0.05)+1.0)*0.5*64);
				//const int alpha=32;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA,alpha);
				DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,color,TRUE);//��
				SetDrawBlendMode(mode,pal);
				//DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,color,FALSE);//�g
			}
		}
		//���j�b�g�̓����蔻��}�`��`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,32);
		GetHitJudgeShape()->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team),TRUE,1.0f);//��
		SetDrawBlendMode(mode,pal);
		GetHitJudgeShape()->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team),FALSE,1.0f);//�g
		//���j�b�g���g�̓����蔻��̕`��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
		m_hitJudgeShape->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team,128,255,255,255),TRUE,1.0f);//��
		m_hitJudgeShape->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team,192,0,0,0),FALSE,3.0f);//�g(����25%������)
		//�I�����j�b�g�̓����蔻�蕔���̋P�x���Z
		if(animationFlag){
			const int addMax=120;
			SetDrawBlendMode(DX_BLENDMODE_ADD,(frame%60)*(60-frame%60)*addMax/900);
			m_hitJudgeShape->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team,128,255,255,255),TRUE,1.0f);//��
			m_hitJudgeShape->Draw(point,adjust,exRate,Team::GetColor(m_battleStatus.team,192,0,0,0),FALSE,3.0f);//�g(����25%������)
		}
		SetDrawBlendMode(mode,pal);
	}
	//�`��͈͂����ɖ߂�
	SetDrawArea(rect.left,rect.top,rect.right,rect.bottom);
	//�A�j���[�V�����p�����[�^�̐ݒ�
	int ux=(int)(pos.x),uy=(int)(pos.y);
	int cx,cy;
	GetGraphSize(m_gHandle,&cx,&cy);
	cx/=2;
	cy/=2;
	double angle=0.0;
	const double defaultRate=1.2*exRate;//�f�t�H�̉摜�g�嗦(�摜�������Ƒ�������W����1.0�ɂ���)
	double exRateX=defaultRate,exRateY=defaultRate;
	if(animationFlag){
		//angle=std::cos(frame/60.0*M_PI)*M_PI/180.0*6.0;
		exRateY=defaultRate-(frame%60)*(60-frame%60)/900.0*0.05;//�{����1.15~1.2�{�A������1�b
		uy+=(int)(cy*(defaultRate-exRateY));//�摜������ɑ�����
	}
	//���j�b�g�O���t�B�b�N��`��
	DrawRotaGraph3(ux,uy,cx,cy,exRateX,exRateY,angle,m_gHandle,TRUE,FALSE);

	//�`�惂�[�h�����ɖ߂�
	SetDrawBlendMode(mode,pal);
}

float Unit::GetMoveDistance()const{
	//�c��OP�ňړ��\�Ȓ������������߂�B
	//return m_battleStatus.OP/CalculateConsumeOP(1.0f)*m_baseStatus.move;
	return GetMoveDistance(m_battleStatus.OP);
}

float Unit::GetMoveDistance(float vOP)const{
	//�c��OP�ňړ��\�Ȓ������������߂�B
	return vOP/CalculateConsumeOP(1.0f)*m_baseStatus.move;
}

const Shape *Unit::GetHitJudgeShape()const{
	if(m_penetratable){
		//�����̍s���t�F�C�Y�Ȃ�΁A���j�b�g���g�̓����蔻��}�`��Ԃ�
		return m_hitJudgeShape.get();
	} else{
		//�G�΃��j�b�g�̍s���t�F�C�Y�Ȃ�A�i���s�̈�̓����蔻��}�`��Ԃ�
		return m_rivalInpenetratableCircle.get();
	}
}

void Unit::Move(Vector2D v){
	//2�̐}�`�𓮂���
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
	//���ɉ������Ȃ�
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
	//�܂����u�p�̐ݒ�����邩�Œ胆�j�b�g�p�̐ݒ�����邩�𔻒肷��
	bool uniqueFlag=false;
	for(const StringBuilder &sb:unitdata.m_vec){
		if(sb.m_vec.size()>=2 && sb.m_vec[0].GetString()=="definition"){
			//�ݒ���@��definition�ɋL�ڂ���Ă���
			uniqueFlag=(sb.m_vec[1].GetString()=="unique");
			break;
		}
	}
	//���u���Œ肩�ŕ���
	if(uniqueFlag){
		//�Œ胆�j�b�g(������)

	} else{
		//���u
		//�e�l��錾�B�ݒ肵�����ǂ�����pair��second�Ɋi�[
		//������͐ݒ�K�{�̂���
		std::optional<std::string> name;
		std::optional<Vector2D> pos;
		std::optional<int> lv;
		std::optional<Unit::Profession::Kind> prof;
		std::optional<std::string> weaponName;
		std::optional<Unit::Team::Kind> team;
		std::optional<Unit::AIType::Kind> aitype;
		std::optional<int> aiGroup;
		std::set<int> aiLinkage;
		//�������͐ݒ�C�ӂ̂���
		std::optional<int> initHP;
		std::optional<float> initOP;
		//�e�l�̓ǂݎ��
		for(const StringBuilder &sb:unitdata.m_vec){
			try{
				if(!sb.m_vec.empty()){
					//�擪�����񂪂��邱�Ƃ�ۏ�
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
						//ai�̃R���}���2�ȏ�̒l�����݂��Ă��ǂ��̂ŁA�����ϐ����󂯎���悤�ɂł��Ă���
						//1�ڂ�AI�̎�ށA2�ڂ͘A���^AI�p�̃O���[�v�l�A3�ڈȍ~�͎��R(���󂷂ׂĂ̒l��aiLinkage�ɓ˂����ނ悤�ɂ��Ă���)
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
				//���l����Ȃ����̂����o�����ꍇ
			} catch(const std::out_of_range &){
				//�\���͈̔͊O�̐��l�����o�����ꍇ
			}
		}
		//�e�l���烆�j�b�g���i�[
		if(name && prof && lv && weaponName && pos && team && aiGroup && aitype){
			//�ݒ�K�{�ł��鍀�ڂ��ݒ肳��Ă��邩
			Unit * const pu=Unit::CreateMobUnit(name.value(),prof.value(),lv.value(),weaponName.value(),pos.value(),team.value(),aitype.value(),aiGroup.value(),aiLinkage);
			//�ݒ�C�ӂł��鍀�ڂ̐ݒ�
			if(initHP && initHP.value()>0 && initHP.value()<pu->GetBattleStatus().HP){
				//HP��0�ȉ���������ő�HP���傫���Ȃ����肵�Ȃ��悤�ɂ���
				pu->AddHP(initHP.value()-pu->GetBattleStatus().HP);
			}
			if(initOP && initOP.value()<Unit::BattleStatus::maxOP){
				//OP��maxOP������Ȃ��悤�ɂ���B0�ȉ��ɂȂ�͖̂��Ȃ�
				pu->SetOP(initOP.value());
			}
			return pu;
		}
	}
	return nullptr;
}
