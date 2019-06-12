#define _USE_MATH_DEFINES
#include"Unit.h"
#include"DxLib.h"
#include"Circle.h"
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"
#include"BattleSceneData.h"
#include<math.h>
#include"FilePath.h"

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

//------------Unit::AIType--------------
Unit::AIType::Kind Unit::AIType::link(int num){
	if(num>=0 && num<END){
		return static_cast<Kind>(num);
	}
	return END;
}

//------------Unit::BattleStatus---------------
const float Unit::BattleStatus::maxOP=100.0f+Unit::reduceStartActionCost+0.0001f;//�L���̗ǂ�������菭�������傫�����鎖��OP��maxOP�܂ő��₷����OP���v�Z�덷�Ŕ��[�Ȑ����ɂȂ�Ȃ��悤�ɂ���B

//------------Unit---------------
const float Unit::unitCircleSize=(float)CommonConstParameter::unitCircleSize;
const float Unit::rivalInpenetratableCircleSize=Unit::unitCircleSize*2.0f;
const float Unit::reduceStartActionCost=50.0f;//�萔�l�Ȃ̂Ńw�b�_�[���������Ɠ������ԂɂȂ�A�����ŏ��������Ă�Unit::BattleStatus::maxOP�̏���������ɏ������s���邱�ƂɂȂ�B
const float Unit::attackCost=50.0f;

const int Unit::hpFontSize=20;

Unit::Unit(BaseStatus baseStatus,std::shared_ptr<Weapon> weapon,Vector2D position,int gHandle,Team::Kind team,AIType::Kind aitype,int aiGroup,std::set<int> aiLinkage)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(position,unitCircleSize,Shape::Fix::e_static)),gHandle)
	,m_baseStatus(baseStatus)
	,m_battleStatus(100,Unit::BattleStatus::maxOP,team,aitype,aiGroup,aiLinkage,weapon)
	,m_rivalInpenetratableCircle(new Circle(position,rivalInpenetratableCircleSize,Shape::Fix::e_static))
//	,m_hpFont(CreateFontToHandleEX("04���񂶂キ�S�V�b�N",hpFontSize,2,DX_FONTTYPE_EDGE,-1,2))
	,m_hpFont(LoadFontDataToHandleEX(FilePath::fontDir+"UnitHPFont.dft",2))
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
	DrawMoveInfo(GetMoveDistance(),point,adjust,GetColor(0,255,255),GetColor(64,192,192));
}

void Unit::DrawMoveInfo(float distance,Vector2D point,Vector2D adjust,unsigned int inColor,unsigned int outColor)const{
	RECT rect;
	GetDrawArea(&rect);
	SetDrawArea(0,0,(int)BattleSceneData::mapDrawSize.x,(int)BattleSceneData::mapDrawSize.y);
	Vector2D pos=point-adjust;
	//���j�b�g�̈ړ����E�����𐅐F�ɕ`��
	DrawCircleAA(pos.x,pos.y,distance,100,outColor,FALSE,3.0f);//�g
	DrawCircleAA(pos.x,pos.y,distance,100,inColor,FALSE,1.0f);//�g
	/*(�d�l���ł̂��߃R�����g�A�E�g)
	//���j�b�g�̍U���\�Ȉړ����E�����𐅐F�ŕ`��(�U���\�ȏꍇ�̂�)
	if((ConsumeOPVirtualByCost(m_battleStatus.weapon->GetCost()))>=0.0f){
	DrawCircleAA(pos.x,pos.y,(ConsumeOPVirtualByCost(m_battleStatus.weapon->GetCost()))*m_baseStatus.move,100,DxLib::GetColor(0,255,255),FALSE);//�g
	}
	//*/
	//�`��͈͂����ɖ߂�
	SetDrawArea(rect.left,rect.top,rect.right,rect.bottom);
}

void Unit::DrawMaxMoveInfo(Vector2D adjust)const{
	DrawMaxMoveInfo(getPos(),adjust);
}

void Unit::DrawMaxMoveInfo(Vector2D point,Vector2D adjust)const{
	DrawMoveInfo(GetMoveDistance(BattleStatus::maxOP-CalculateConsumeOP(reduceStartActionCost)),point,adjust,GetColor(0,128,255),GetColor(64,128,192));
}

void Unit::DrawHPGage(Vector2D adjust)const{
	DrawHPGage(getPos(),adjust);
}

void Unit::DrawHPGage(Vector2D point,Vector2D adjust)const{
	//HP�Q�[�W��HP�̕\���B�Q�[�W�͔�AA�ŕ`�悵���ق����Y��Ɍ�����
	const int gageX=(int)(getPos().x-unitCircleSize),gageY=(int)(getPos().y+unitCircleSize)-hpFontSize/2,unitCircleSizeInteger=(int)(unitCircleSize),margin=2;
	const int gageMaxLength=(unitCircleSizeInteger-margin)*2;
	const int gageLength=gageMaxLength*m_battleStatus.HP/m_baseStatus.maxHP;
	//�Q�[�W�̐F
	unsigned int color;
/*
	//HP�̊����ŐF�����߂�
	if(gageLength>gageMaxLength*3/4){
		//HP���S�̂�3/4�ȏ�Ȃ琅�F
		color=GetColor(0,196,255);
	} else if(gageLength>gageMaxLength*2/4){
		//HP���S�̂�1/2�ȏ�Ȃ物�ΐF
		color=GetColor(32,196,0);
	} else if(gageLength>gageMaxLength*1/4){
		//HP���S�̂�1/4�ȏ�Ȃ物�F
		color=GetColor(196,196,0);
	} else{
		//HP���S�̂�1/4�ȉ��Ȃ�ԐF
		color=GetColor(255,64,0);
	}
//*/
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

void Unit::DrawUnit(Vector2D adjust,size_t frame,bool animationFlag,bool infoDrawFlag)const{
	DrawUnit(getPos(),adjust,frame,animationFlag,infoDrawFlag);
}

void Unit::DrawUnit(Vector2D point,Vector2D adjust,size_t frame,bool animationFlag,bool infoDrawFlag)const{
	Vector2D pos=point-adjust;
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	if(infoDrawFlag){
		//�A�N�V�����̌��ʔ͈͂𔼓���(��)�ŕ`��
		//�ЂƂ܂��Z�˒��ŕ`��{���͕���N���X��Draw�֐����g���̂��]�܂����B
		if(GetFix()==Shape::Fix::e_dynamic){
			//dynamic�ȃL�����̂݃A�N�V�����͈͂�\���B���炭�ړ����Ă���L�����̂�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,32);
			DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,Team::GetColor(m_battleStatus.team),TRUE);//��
			SetDrawBlendMode(mode,pal);
			DrawCircleAA(pos.x,pos.y,m_battleStatus.weapon->GetLength(),100,Team::GetColor(m_battleStatus.team),FALSE);//�g
		}
		//���j�b�g�̓����蔻��}�`��`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,32);
		GetHitJudgeShape()->Draw(pos,adjust,Team::GetColor(m_battleStatus.team),TRUE);//��
		SetDrawBlendMode(mode,pal);
		GetHitJudgeShape()->Draw(pos,adjust,Team::GetColor(m_battleStatus.team),FALSE);//�g
		//���j�b�g���g�̓����蔻��̕`��
//		SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
		m_hitJudgeShape->Draw(pos,adjust,Team::GetColor(m_battleStatus.team,128,255,255,255),TRUE);//��
		m_hitJudgeShape->Draw(pos,adjust,Team::GetColor(m_battleStatus.team,192,0,0,0),FALSE,3);//�g(����25%������)
		//�I�����j�b�g�̓����蔻�蕔���̋P�x���Z
		if(animationFlag){
			const int addMax=120;
			SetDrawBlendMode(DX_BLENDMODE_ADD,(frame%60)*(60-frame%60)*addMax/900);
			m_hitJudgeShape->Draw(pos,adjust,Team::GetColor(m_battleStatus.team,128,255,255,255),TRUE);//��
			m_hitJudgeShape->Draw(pos,adjust,Team::GetColor(m_battleStatus.team,192,0,0,0),FALSE,3);//�g(����25%������)
		}
		SetDrawBlendMode(mode,pal);
	}
	//�A�j���[�V�����p�����[�^�̐ݒ�
	int ux=(int)(pos.x),uy=(int)(pos.y);
	int cx,cy;
	GetGraphSize(m_gHandle,&cx,&cy);
	cx/=2;
	cy/=2;
	double angle=0.0;
	const double defaultRate=1.2;//�f�t�H�̉摜�g�嗦(�摜�������Ƒ�������1.0�ɂ���)
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
	DrawUnit(point,adjust,0,false,true);
}

void Unit::VHitProcess(const BattleObject *potherobj){
	//���ɉ������Ȃ�
}

std::shared_ptr<BattleObject> Unit::VCopy()const{
	return std::shared_ptr<BattleObject>(new Unit(*this));
}

Unit *Unit::CreateMobUnit(std::string name,Profession::Kind profession,int lv,Vector2D position,Team::Kind team,AIType::Kind aitype,int aiGroup,std::set<int> aiLinkage){
	BaseStatus baseStatus;
	std::shared_ptr<Weapon> weapon;
	int gHandle=-1;
	switch(profession){
	case(Profession::e_soldier):
		baseStatus=BaseStatus(name,profession,lv,20+(int)(lv*0.8),6+(int)(lv*0.5),5+(int)(lv*0.45),2+(int)(lv*0.1),4+(int)(lv*0.4),5+(int)(lv*0.5),6);
		weapon=Weapon::GetWeapon("�S�̌�");
		gHandle=LoadGraphEX("Graphic/nonfree/soldier.png");
		break;
	case(Profession::e_archer):
		baseStatus=BaseStatus(name,profession,lv,18+(int)(lv*0.75),5+(int)(lv*0.45),4+(int)(lv*0.4),2+(int)(lv*0.1),4+(int)(lv*0.4),3+(int)(lv*0.3),6);
		weapon=Weapon::GetWeapon("�S�̋|");
		gHandle=LoadGraphEX("Graphic/nonfree/archer.png");
		break;
	case(Profession::e_armer):
		baseStatus=BaseStatus(name,profession,lv,25+(int)(lv*0.9),7+(int)(lv*0.6),7+(int)(lv*0.6),0+(int)(lv*0.1),0+(int)(lv*0.2),1+(int)(lv*0.2),3);
		weapon=Weapon::GetWeapon("�S�̑�");
		gHandle=LoadGraphEX("Graphic/nonfree/armerknight.png");
		break;
	case(Profession::e_mage):
		baseStatus=BaseStatus(name,profession,lv,16+(int)(lv*0.6),1+(int)(lv*0.1),1+(int)(lv*0.2),6+(int)(lv*0.5),6+(int)(lv*0.45),5+(int)(lv*0.5),4);
		weapon=Weapon::GetWeapon("�t�@�C�A�[�̏�");
		gHandle=LoadGraphEX("Graphic/nonfree/mage.png");
		break;
	case(Profession::e_healer):
		baseStatus=BaseStatus(name,profession,lv,13+(int)(lv*0.5),0+(int)(lv*0.1),1+(int)(lv*0.2),6+(int)(lv*0.5),7+(int)(lv*0.5),4+(int)(lv*0.4),6);
		weapon=Weapon::GetWeapon("�q�[���̏�");
		gHandle=LoadGraphEX("Graphic/nonfree/healer.png");
		break;
	}
	return new Unit(baseStatus,weapon,position,gHandle,team,aitype,aiGroup,aiLinkage);
}
