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
{
	//m_operateUnitの図形状態をdynamicにする
	m_operateUnit->SetFix(Shape::Fix::e_dynamic);
}

ReflectionWork::MoveSimulation::~MoveSimulation(){}

void ReflectionWork::MoveSimulation::Update(){
	//m_operateUnitを動かす
	const Vector2D mouseMapPosition=(GetMousePointVector2D()-m_startPos)/m_mapRate;
	UpdateOperateUnitPosition(mouseMapPosition-m_operateUnit->getPos());
	//評価の更新
	m_evaluate=m_rule->RubricEvaluate(m_field,m_stageSize,CreateLog());
	//終了更新
	if(mouse_get(MOUSE_INPUT_LEFT)==1 || keyboard_get(KEY_INPUT_Z)==1){
		//決定ボタンをクリックしていて
		if(m_attackedUnit==nullptr || m_operateUnit->JudgeAttackable(m_attackedUnit)){
			//行動可能な場合（元々行動しない場合は無条件で）
			m_clear=true;
			//m_operateUnitの図形状態をstaticに戻す
			m_operateUnit->SetFix(Shape::Fix::e_static);
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

void ReflectionWork::MoveSimulation::UpdateOperateUnitPosition(const Vector2D inputVec){
	//各値の定義
	const float speed=(float)(m_operateUnit->GetBaseStatus().move);//オブジェクトの移動速度
	const size_t moveCount=5;//移動回数の分割数
	const size_t judgeCount=3;//1移動内の当たり判定実行回数
	const Vector2D beforePos=m_operateUnit->getPos();//移動前の位置を取得
	//移動ベクトルの計算
	const Vector2D moveVec=inputVec.norm()*std::fminf(speed,inputVec.size())/((float)moveCount);
	//位置更新作業
	for(size_t i=0;i<moveCount;i++){
		//1フレーム内に5回移動
		//移動処理を小分けにする事で、移動速度を向上させることができる
		//1回の移動距離を小さくしないとギリギリ通れない場所が通れるようになってしまう
		m_operateUnit->Move(moveVec);
		//1フレーム内に複数回当たり判定処理を行うと、処理が重くなる代わりにオブジェクトの移動速度を上げることができる
		for(BattleObject *pObject:m_field){
			//当たり判定系の処理
			pObject->UpdatePosition(std::vector<ShapeHaving *>(m_field.begin(),m_field.end()).data(),m_field.size(),judgeCount);
		}
	}
}
