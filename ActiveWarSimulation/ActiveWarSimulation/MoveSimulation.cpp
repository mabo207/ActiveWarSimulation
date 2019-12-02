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
	,m_font(CreateFontToHandleEX("メイリオ",24,3,DX_FONTTYPE_EDGE,-1,2))
{}

ReflectionWork::MoveSimulation::~MoveSimulation(){}

void ReflectionWork::MoveSimulation::Update(){
	//m_operateUnitを動かす(暫定)
	const Vector2D mouse=GetMousePointVector2D();
	const Vector2D newPosition=(mouse-m_startPos)/m_mapRate;
	m_operateUnit->Warp(newPosition);
	//評価の更新
	m_evaluate=m_rule->RubricEvaluate(m_field,m_stageSize,CreateLog());
	//終了更新
	if(mouse_get(MOUSE_INPUT_LEFT)==1 || keyboard_get(KEY_INPUT_Z)==1){
		//決定ボタンをクリックしていて
		if(m_attackedUnit==nullptr || m_operateUnit->JudgeAttackable(m_attackedUnit)){
			//行動可能な場合（元々行動しない場合は無条件で）
			m_clear=true;
		}
	}
}

bool ReflectionWork::MoveSimulation::WorkClear()const{
	return m_clear;
}

void ReflectionWork::MoveSimulation::WorkDraw()const{
	//評価の描画
	const Vector2D pos=m_operateUnit->getPos()*m_mapRate+m_startPos+Vector2D(0.0f,-Unit::unitCircleSize);
	DrawStringCenterBaseToHandle((int)pos.x,(int)pos.y,m_evaluate.GetString().c_str(),GetColor(255,255,255),m_font,true,m_evaluate.Color());
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
