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
const float Unit::BattleStatus::maxOP=100.0f+0.0001f;//�L���̗ǂ�������菭�������傫�����鎖��OP��maxOP�܂ő��₷����OP���v�Z�덷�Ŕ��[�Ȑ����ɂȂ�Ȃ��悤�ɂ���B

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

bool Unit::SetPenetratable(Team::Kind nowPhase){
	return (m_penetratable=(m_battleStatus.team==nowPhase));
}

bool Unit::JudgeAttackable(const Unit *pUnit)const{
	if(GetBattleStatus().team==pUnit->GetBattleStatus().team){
		//�����`�[���Ȃ�U���ł��Ȃ�
		return false;
	}
	//�U���̎˒��ƈʒu�֌W�ɂ�����
	std::shared_ptr<Shape> pWeapon(new Circle(getPos(),closeAttackLength,Shape::Fix::e_dynamic));
	if(pWeapon->CalculatePushVec(pUnit->GetUnitCircleShape())==Vector2D()){
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

void Unit::AddOP(float cost){
	m_battleStatus.OP+=cost;
}

float Unit::CalculateAddOPNormalAttack()const{
	//����ɂ���Č��܂邪�A�ЂƂ܂����ߑł��̒l��Ԃ�
	return -attackCost;
}

void Unit::DrawMoveInfo(Vector2D adjust)const{
	DrawMoveInfo(getPos(),adjust);
}

void Unit::DrawMoveInfo(Vector2D point,Vector2D adjust)const{
	Vector2D pos=point-adjust;
	//���j�b�g�̈ړ����E������΂�`��
	DrawCircleAA(pos.x,pos.y,m_battleStatus.OP*m_baseStatus.move,100,DxLib::GetColor(0,255,0),FALSE);//�g
	//���j�b�g�̍U���\�Ȉړ����E�����𐅐F�ŕ`��(�U���\�ȏꍇ�̂�)
	if(m_battleStatus.OP>Unit::attackCost){
		DrawCircleAA(pos.x,pos.y,(m_battleStatus.OP-Unit::attackCost)*m_baseStatus.move,100,DxLib::GetColor(0,255,255),FALSE);//�g
	}

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
	Vector2D pos=point-adjust;
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	//�A�N�V�����̌��ʔ͈͂𔼓���(��)�ŕ`��
	//�ЂƂ܂��Z�˒��ŕ`��{���͕���N���X��Draw�֐����g���̂��]�܂����B
	if(GetFix()==Shape::Fix::e_dynamic){
		//dynamic�ȃL�����̂݃A�N�V�����͈͂�\���B���炭�ړ����Ă���L�����̂�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
		DrawCircleAA(pos.x,pos.y,closeAttackLength,100,Team::GetColor(m_battleStatus.team),TRUE);//��
		SetDrawBlendMode(mode,pal);
		DrawCircleAA(pos.x,pos.y,closeAttackLength,100,Team::GetColor(m_battleStatus.team),FALSE);//�g
	}
	//���j�b�g�̓����蔻��}�`��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
	GetHitJudgeShape()->Draw(adjust,Team::GetColor(m_battleStatus.team),TRUE);//��
	SetDrawBlendMode(mode,pal);
	GetHitJudgeShape()->Draw(adjust,Team::GetColor(m_battleStatus.team),FALSE);//�g
	//���j�b�g���g�̓����蔻��̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
	m_hitJudgeShape->Draw(adjust,Team::GetColor(m_battleStatus.team),TRUE);//��
	SetDrawBlendMode(mode,pal);
	m_hitJudgeShape->Draw(adjust,Team::GetColor(m_battleStatus.team),FALSE);//��
	//���j�b�g�O���t�B�b�N��`��

	//HP�Q�[�W��HP�̕\���B�Q�[�W�͔�AA�ŕ`�悵���ق����Y��Ɍ�����
	const int gageX=(int)(getPos().x-unitCircleSize),gageY=(int)(getPos().y+unitCircleSize)-hpFontSize/2;
	DrawBox(gageX,gageY,gageX+(int)(unitCircleSize*2),gageY+hpFontSize,GetColor(0,0,0),TRUE);//�Q�[�W�O��
	DrawBox(gageX+1,gageY+1,gageX+(int)(unitCircleSize)*2*m_battleStatus.HP/m_baseStatus.maxHP-1,gageY+hpFontSize-1,GetColor(0,128,255),TRUE);//�Q�[�W����
	DrawStringRightJustifiedToHandle(gageX,gageY,std::to_string(m_battleStatus.HP),GetColor(255,255,255),m_hpFont,GetColor(0,0,0));//HP�̕�����
	//�`�惂�[�h�����ɖ߂�
	SetDrawBlendMode(mode,pal);
}

void Unit::VHitProcess(const BattleObject *potherobj){
	//���ɉ������Ȃ�
}

std::shared_ptr<BattleObject> Unit::VCopy()const{
	return std::shared_ptr<BattleObject>(new Unit(m_hitJudgeShape->GetPosition(),m_gHandle,m_battleStatus.team));
}
