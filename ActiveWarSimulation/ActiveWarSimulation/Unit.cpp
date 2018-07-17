#include"Unit.h"
#include"DxLib.h"
#include"Circle.h"
#include"GraphicControl.h"
#include"ToolsLib.h"

//------------Unit::Profession---------------
const std::map<std::string,Unit::Profession::Kind> Unit::Profession::professionMap={
	std::pair<std::string,Unit::Profession::Kind>("���p�m",Unit::Profession::e_lancer)
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
		//�`�[�����������Ȃ疡��
		return true;
	}
	//���̑������ł���g�ݍ��킹���


	//������ł��Ȃ��Ȃ�G
	return false;
}

//------------Unit::BattleStatus---------------
const float Unit::BattleStatus::maxOP=100.0f+Unit::reduceStartActionCost+0.0001f;//�L���̗ǂ�������菭�������傫�����鎖��OP��maxOP�܂ő��₷����OP���v�Z�덷�Ŕ��[�Ȑ����ɂȂ�Ȃ��悤�ɂ���B

//------------Unit---------------
const float Unit::unitCircleSize=45.0f;
const float Unit::rivalInpenetratableCircleSize=Unit::unitCircleSize*2.0f;
const float Unit::reduceStartActionCost=50.0f;

const float Unit::attackCost=50.0f;

const int Unit::hpFontSize=20;

Unit::Unit(Vector2D position,int gHandle,Team::Kind team)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(position,unitCircleSize,Shape::Fix::e_static)),gHandle)
	,m_baseStatus("����",Profession::e_lancer,2,20,5,3,2,3,5,4)
	,m_battleStatus(20,0,team,Weapon::GetWeapon("�S�̑�"))
	,m_rivalInpenetratableCircle(new Circle(position,rivalInpenetratableCircleSize,Shape::Fix::e_static))
	,m_hpFont(CreateFontToHandleEX("���C���I",hpFontSize,1,DX_FONTTYPE_EDGE))
{
	//�e�X�g�p�̃R���X�g���N�^
	m_battleStatus.HP=m_baseStatus.maxHP;
}

Unit::Unit(BaseStatus baseStatus,std::shared_ptr<Weapon> weapon,Vector2D position,int gHandle,Team::Kind team)
	:BattleObject(Type::e_unit,std::shared_ptr<Shape>(new Circle(position,unitCircleSize,Shape::Fix::e_static)),gHandle)
	,m_baseStatus(baseStatus),m_battleStatus(100,Unit::BattleStatus::maxOP,team,weapon)
	,m_rivalInpenetratableCircle(new Circle(position,rivalInpenetratableCircleSize,Shape::Fix::e_static))
	,m_hpFont(CreateFontToHandleEX("���C���I",hpFontSize,1,DX_FONTTYPE_EDGE))
{
	//�e�X�g�p�̃R���X�g���N�^
	m_battleStatus.HP=m_baseStatus.maxHP;
}

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

void Unit::DrawMoveInfo(Vector2D adjust)const{
	DrawMoveInfo(getPos(),adjust);
}

void Unit::DrawMoveInfo(Vector2D point,Vector2D adjust)const{
	Vector2D pos=point-adjust;
	//���j�b�g�̈ړ����E������΂�`��
	DrawCircleAA(pos.x,pos.y,GetMoveDistance(),100,DxLib::GetColor(0,255,0),FALSE);//�g
	//���j�b�g�̍U���\�Ȉړ����E�����𐅐F�ŕ`��(�U���\�ȏꍇ�̂�)
	if(m_battleStatus.OP>Unit::attackCost){
		DrawCircleAA(pos.x,pos.y,(m_battleStatus.OP-Unit::attackCost)*m_baseStatus.move,100,DxLib::GetColor(0,255,255),FALSE);//�g
	}

}

void Unit::DrawHPGage(Vector2D adjust)const{
	DrawHPGage(getPos(),adjust);
}

void Unit::DrawHPGage(Vector2D point,Vector2D adjust)const{
	//HP�Q�[�W��HP�̕\���B�Q�[�W�͔�AA�ŕ`�悵���ق����Y��Ɍ�����
	const int gageX=(int)(getPos().x-unitCircleSize),gageY=(int)(getPos().y+unitCircleSize)-hpFontSize/2,unitCircleSizeInteger=(int)(unitCircleSize),margin=2;
	const int gageMaxLength=(unitCircleSizeInteger-margin)*2;
	const int gageLength=gageMaxLength*m_battleStatus.HP/m_baseStatus.maxHP;
	unsigned int color;//�Q�[�W�̐F
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
	DrawBox(gageX,gageY,gageX+unitCircleSizeInteger*2,gageY+hpFontSize,GetColor(0,0,0),TRUE);//�Q�[�W�O��
	DrawBox(gageX+margin,gageY+margin,gageX+margin+gageLength,gageY+hpFontSize-margin,color,TRUE);//�Q�[�W����
	DrawStringRightJustifiedToHandle(gageX,gageY,std::to_string(m_battleStatus.HP),GetColor(255,255,255),m_hpFont,GetColor(0,0,0));//HP�̕�����

}

void Unit::DrawFacePic(Vector2D point)const{
	//�~�̕`��
	const int x=(int)point.x,y=(int)point.y,r=(int)unitCircleSize;
	DrawCircle(x,y,r,Team::GetColor(m_battleStatus.team),TRUE);//�w�i�̉~�̕`��
	DrawRotaGraph(x,y,1.0,0.0,m_gHandle,TRUE);//�O���t�B�b�N�̕`��A�b��Ń}�b�v��̃��j�b�g�G���g�p
	DrawCircle(x,y,r,GetColor(255,255,255),FALSE,3);//�w�i�̘g�̕`��
}

void Unit::DrawUnit(Vector2D point,Vector2D adjust,bool infoDrawFlag)const{
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
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
		m_hitJudgeShape->Draw(pos,adjust,Team::GetColor(m_battleStatus.team),TRUE);//��
		SetDrawBlendMode(mode,pal);
		m_hitJudgeShape->Draw(pos,adjust,Team::GetColor(m_battleStatus.team),FALSE);//��
	}
	//���j�b�g�O���t�B�b�N��`��
	DrawRotaGraph((int)(pos.x),(int)(pos.y),1.0,0.0,m_gHandle,TRUE,FALSE);
	//�`�惂�[�h�����ɖ߂�
	SetDrawBlendMode(mode,pal);
}

float Unit::GetMoveDistance()const{
	//�c��OP�ňړ��\�Ȓ������������߂�B
	return m_battleStatus.OP/CalculateConsumeOP(1.0f)*m_baseStatus.move;
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
	DrawUnit(point,adjust,true);
}

void Unit::VHitProcess(const BattleObject *potherobj){
	//���ɉ������Ȃ�
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
		weapon=Weapon::GetWeapon("�S�̑�");
		gHandle=LoadGraphEX("Graphic/soldier.png");
		break;
	case(Profession::e_archer):
		baseStatus=BaseStatus(name,profession,lv,18+(int)(lv*0.75),4+(int)(lv*0.45),3+(int)(lv*0.3),2+(int)(lv*0.1),3+(int)(lv*0.3),3+(int)(lv*0.3),6);
		weapon=Weapon::GetWeapon("�S�̋|");
		gHandle=LoadGraphEX("Graphic/archer.png");
		break;
	case(Profession::e_armer):
		baseStatus=BaseStatus(name,profession,lv,25+(int)(lv*0.9),6+(int)(lv*0.6),6+(int)(lv*0.6),0+(int)(lv*0.1),0+(int)(lv*0.1),1+(int)(lv*0.2),3);
		weapon=Weapon::GetWeapon("�S�̑�");
		gHandle=LoadGraphEX("Graphic/armerknight.png");
		break;
	case(Profession::e_mage):
		baseStatus=BaseStatus(name,profession,lv,16+(int)(lv*0.6),1+(int)(lv*0.1),1+(int)(lv*0.2),6+(int)(lv*0.6),5+(int)(lv*0.4),5+(int)(lv*0.5),4);
		weapon=Weapon::GetWeapon("�t�@�C�A�[�̏�");
		gHandle=LoadGraphEX("Graphic/mage.png");
		break;
	case(Profession::e_healer):
		baseStatus=BaseStatus(name,profession,lv,13+(int)(lv*0.5),0+(int)(lv*0.1),1+(int)(lv*0.2),5+(int)(lv*0.55),7+(int)(lv*0.5),4+(int)(lv*0.4),6);
		weapon=Weapon::GetWeapon("�q�[���̏�");
		gHandle=LoadGraphEX("Graphic/healer.png");
		break;
	}
	return new Unit(baseStatus,weapon,position,gHandle,team);
}
