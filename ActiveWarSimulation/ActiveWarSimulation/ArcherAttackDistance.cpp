#include"DxLib.h"
#include"ArcherAttackDistance.h"
#include"BattleSceneData.h"
#include"AttackLog.h"

int ArcherAttackDistance::RubricEvaluate(const BattleSceneData * const battleData)const{
	//- 例外処理
	//	- 攻撃しない(-1)
	//- 評価
	//	0. 直線距離が、敵ユニット攻撃射程以下(敵の射程が自分の射程より長い場合は無条件で達成するものとする)
	//	1. ルート距離が、自ユニット攻撃射程以下
	//	2. ルート距離が、敵の移動距離以下
	//	3. 2以上の評価
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(battleData->m_scoreObserver->GetLatestLog());
	int evaluate;
	if(!attackLog){
		//ログがAttackLogでない場合は「攻撃をしなかった」と判断できる
		evaluate=-1;
	} else{
		//距離に関する評価をする
		//直線距離を求める
		const float directDistance=(attackLog->GetOperateUnitData().pos-attackLog->GetAimedUnitData().pos).size();
		//被弾ユニット→行動ユニットへのルート距離を求める
		const Unit *operatedUnit=attackLog->GetOperateUnitData().punit;
		const Unit *aimedUnit=attackLog->GetAimedUnit();
		const float routeDistance=CalculateRouteDistance(battleData,attackLog->m_unitDataList,operatedUnit,aimedUnit);
		//評価(高い方から判定していく)
		if(routeDistance>=attackLog->GetAimedUnit()->GetMaxMoveDistance()){
			evaluate=3;
		} else if(routeDistance>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength()){
			evaluate=2;
		} else if(directDistance>=attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()
			|| attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength())
		{
			evaluate=1;
		} else{
			evaluate=0;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> ArcherAttackDistance::GetRubricStringInfo(int rubric)const{
	std::string rubricStr;
	unsigned int edgeColor;
	switch(rubric){
	case(-1):
		rubricStr="";
		edgeColor=GetColor(0,0,0);
		break;
	case(0):
		//悪い
		rubricStr="Worst";
		edgeColor=GetColor(128,0,196);
		break;
	case(1):
		//悪い
		rubricStr="Bad";
		edgeColor=GetColor(96,96,196);
		break;
	case(2):
		//微妙
		rubricStr="Not good";
		edgeColor=GetColor(128,128,196);
		break;
	case(3):
		//完璧
		rubricStr="Good!!";
		edgeColor=GetColor(196,196,64);
		break;
	}
	return std::make_pair(rubricStr,edgeColor);
}

std::string ArcherAttackDistance::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//攻撃していない
		comment="もっと射手で攻撃してみよう！";
		break;
	case(0):
		//敵ユニットの攻撃射程内からの攻撃
		comment="敵の射程外から攻撃する事を意識してみると良いよ！";
		break;
	case(1):
		//ルート距離が攻撃射程以下
		comment="弓で攻撃する時は、障害物や味方越しに攻撃してみよう！";
		break;
	case(2):
		//ルート距離が敵の移動距離以下
		comment="敵が次の行動でどこまで行けるかを考えて、敵の攻撃を受けないような位置で攻撃することを意識してみよう！";
		break;
	case(3):
		//これ以上の評価
		comment="言うことなしです！障害物や味方をうまく使って、安全な場所から攻撃できています！";
		break;
	}
	return comment;
}

std::string ArcherAttackDistance::GetWholeLookBackActionEmpty()const{
	return "もっと射手で攻撃してみよう！";
}

std::string ArcherAttackDistance::GetSubmissionExplanation()const{
	return "地形や味方キャラの壁を利用して、\n安全地帯から攻撃してみよう！";
}

bool ArcherAttackDistance::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_archer);
}
