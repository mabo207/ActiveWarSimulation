#include"MoveSimulation.h"
#include"DxLib.h"
#include"AttackLog.h"
#include"WaitLog.h"
#include"input.h"
#include"GraphicControl.h"
#include"ToolsLib.h"

//-------------------ReflectionWork::MoveSimulation-------------------
ReflectionWork::MoveSimulation::MoveSimulation(const std::vector<BattleObject *> &field
	,Unit * const operateUnit
	,const Vector2D stageSize
	,const Unit * const attackedUnit
	,const Vector2D startPos
	,const float mapRate
	,const std::shared_ptr<const SubmissionRuleBase> &rule
	,const std::string &question)
	:Base(Kind::e_moveSimulation,question)
	,m_field(field)
	,m_stageSize(stageSize)
	,m_operateUnit(operateUnit)
	,m_startPos(startPos)
	,m_mapRate(mapRate)
	,m_attackedUnit(attackedUnit)
	,m_rule(rule)
	,m_clear(false)
	,m_font(CreateFontToHandleEX("���C���I",24,3,DX_FONTTYPE_EDGE,-1,2))
{
	//m_operateUnit�̐}�`��Ԃ�dynamic�ɂ���
	m_operateUnit->SetFix(Shape::Fix::e_dynamic);
}

ReflectionWork::MoveSimulation::~MoveSimulation(){}

void ReflectionWork::MoveSimulation::Update(){
	//m_operateUnit�𓮂���
	const Vector2D mouseMapPosition=(GetMousePointVector2D()-m_startPos)/m_mapRate;
	UpdateOperateUnitPosition(mouseMapPosition-m_operateUnit->getPos());
	//�]���̍X�V
	m_evaluate=m_rule->RubricEvaluate(m_field,m_stageSize,CreateLog());
	//�I���X�V
	if(mouse_get(MOUSE_INPUT_LEFT)==1 || keyboard_get(KEY_INPUT_Z)==1){
		//����{�^�����N���b�N���Ă���
		if(m_attackedUnit==nullptr || m_operateUnit->JudgeAttackable(m_attackedUnit)){
			//�s���\�ȏꍇ�i���X�s�����Ȃ��ꍇ�͖������Łj
			m_clear=true;
			//m_operateUnit�̐}�`��Ԃ�static�ɖ߂�
			m_operateUnit->SetFix(Shape::Fix::e_static);
		}
	}
}

bool ReflectionWork::MoveSimulation::WorkClear()const{
	return m_clear;
}

void ReflectionWork::MoveSimulation::WorkDraw()const{
	//�]���̕`��
	const Vector2D pos=m_operateUnit->getPos()*m_mapRate+m_startPos+Vector2D(0.0f,-Unit::unitCircleSize);
	DrawStringCenterBaseToHandle((int)pos.x,(int)pos.y,m_evaluate.GetString().c_str(),GetColor(255,255,255),m_font,true,m_evaluate.Color());
}

std::shared_ptr<LogElement> ReflectionWork::MoveSimulation::CreateLog()const{
	//unitList�̍\��
	std::vector<Unit *> unitList;
	for(BattleObject *obj:m_field){
		//HP���c���Ă��郆�j�b�g�������o
		if(obj->GetType()==BattleObject::Type::e_unit){
			Unit *unit=dynamic_cast<Unit *>(obj);
			if(unit!=nullptr && unit->GetBattleStatus().HP>0){
				unitList.push_back(unit);
			}
		}
	}
	//���O�̍\��
	if(m_attackedUnit!=nullptr && m_operateUnit->JudgeAttackable(m_attackedUnit)){
		//�U���\�ȏꍇ(������)
		return std::shared_ptr<LogElement>(new AttackLog(unitList,m_attackedUnit,{}));
	} else{
		//�U���s�ȏꍇ(������)
		return std::shared_ptr<LogElement>(new WaitLog(unitList,{}));
	}
}

void ReflectionWork::MoveSimulation::UpdateOperateUnitPosition(const Vector2D inputVec){
	//�e�l�̒�`
	const float speed=(float)(m_operateUnit->GetBaseStatus().move);//�I�u�W�F�N�g�̈ړ����x
	const size_t moveCount=5;//�ړ��񐔂̕�����
	const size_t judgeCount=3;//1�ړ����̓����蔻����s��
	const Vector2D beforePos=m_operateUnit->getPos();//�ړ��O�̈ʒu���擾
	//�ړ��x�N�g���̌v�Z
	const Vector2D moveVec=inputVec.norm()*std::fminf(speed,inputVec.size())/((float)moveCount);
	//�ʒu�X�V���
	for(size_t i=0;i<moveCount;i++){
		//1�t���[������5��ړ�
		//�ړ��������������ɂ��鎖�ŁA�ړ����x�����コ���邱�Ƃ��ł���
		//1��̈ړ����������������Ȃ��ƃM���M���ʂ�Ȃ��ꏊ���ʂ��悤�ɂȂ��Ă��܂�
		m_operateUnit->Move(moveVec);
		//1�t���[�����ɕ����񓖂��蔻�菈�����s���ƁA�������d���Ȃ����ɃI�u�W�F�N�g�̈ړ����x���グ�邱�Ƃ��ł���
		for(BattleObject *pObject:m_field){
			//�����蔻��n�̏���
			pObject->UpdatePosition(std::vector<ShapeHaving *>(m_field.begin(),m_field.end()).data(),m_field.size(),judgeCount);
		}
	}
}
