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
	//�e�X�g�p�̃R���X�g���N�^
	m_battleStatus.HP=m_baseStatus.maxHP;
}

Unit::~Unit(){}

void Unit::WriteOutObjectPeculiarInfo(std::ofstream &ofs)const{
	//�b��
	ofs<<"("<<Type::GetStr(m_type)<<")";
}

bool Unit::SetPenetratable(Team::Kind nowPhase){
	return (m_penetratable=(m_battleStatus.team==nowPhase));
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
	SetDrawBlendMode(mode,pal);

	//�`�惂�[�h�����ɖ߂��Ă���
}

void Unit::VHitProcess(const BattleObject *potherobj){
	//���ɉ������Ȃ�
}

std::shared_ptr<BattleObject> Unit::VCopy()const{
	return std::shared_ptr<BattleObject>(new Unit(m_hitJudgeShape->GetPosition(),m_gHandle,m_battleStatus.team));
}
