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
	,m_font(CreateFontToHandleEX("メイリオ",24,3,DX_FONTTYPE_EDGE))
{}

ReflectionWork::MoveSimulation::~MoveSimulation(){}

void ReflectionWork::MoveSimulation::Update(){
	//m_operateUnitを動かす(暫定)
	const Vector2D mouse=GetMousePointVector2D();
	const Vector2D newPosition=(mouse-m_startPos)/m_mapRate;
	m_operateUnit->Warp(newPosition);
}

bool ReflectionWork::MoveSimulation::WorkClear()const{
	return m_clear;
}

void ReflectionWork::MoveSimulation::WorkDraw()const{
	//評価
	const int evaluate=m_rule->RubricEvaluate(m_field,m_stageSize,CreateLog());
	const auto info=m_rule->GetRubricStringInfo(evaluate);
	//評価の描画
	const Vector2D pos=m_operateUnit->getPos()*m_mapRate+m_startPos+Vector2D(0.0f,-Unit::unitCircleSize);
	DrawStringCenterBaseToHandle((int)pos.x,(int)pos.y,info.first.c_str(),GetColor(255,255,255),m_font,true,info.second);
}

std::shared_ptr<LogElement> ReflectionWork::MoveSimulation::CreateLog()const{
	//unitListの構成
	std::vector<Unit *> unitList;
	for(BattleObject *obj:m_field){
		//HPが残っているユニットだけ抽出
		if(obj->GetType()==BattleObject::Type::e_unit){
			Unit *unit=dynamic_cast<Unit *>(obj);
			if(unit!=nullptr && unit->GetBattleStatus().HP>0){
				unitList.push_back(unit);
			}
		}
	}
	//ログの構成
	if(m_attackedUnit!=nullptr && m_operateUnit->JudgeAttackable(m_attackedUnit)){
		//攻撃可能な場合(未完成)
		return std::shared_ptr<LogElement>(new AttackLog(unitList,m_attackedUnit,{}));
	} else{
		//攻撃不可な場合(未完成)
		return std::shared_ptr<LogElement>(new WaitLog(unitList,{}));
	}
}
