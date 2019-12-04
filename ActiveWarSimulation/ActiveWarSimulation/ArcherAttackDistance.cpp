#include"DxLib.h"
#include"ArcherAttackDistance.h"
#include"BattleSceneData.h"
#include"AttackLog.h"

//----------------ArcherAttackDistance----------------
const std::array<SubmissionEvaluation,4> ArcherAttackDistance::s_evaluate={
	SubmissionEvaluation::e_bad
	,SubmissionEvaluation::e_ok
	,SubmissionEvaluation::e_good
	,SubmissionEvaluation::e_excellent
};

SubmissionEvaluation ArcherAttackDistance::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- 例外処理
	//	- 攻撃しない(-1)
	//- 評価
	//	0. 直線距離が、敵ユニット攻撃射程以下(敵の射程が自分の射程より長い場合は無条件で達成するものとする)
	//	1. ルート距離が、自ユニット攻撃射程以下
	//	2. ルート距離が、敵の移動距離以下
	//	3. 2以上の評価
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	SubmissionEvaluation evaluate;
	if(!attackLog){
		//ログがAttackLogでない場合は「攻撃をしなかった」と判断できる
		evaluate=SubmissionEvaluation::e_noevaluation;
	} else{
		//距離に関する評価をする
		//直線距離を求める
		const float directDistance=(attackLog->GetOperateUnitData().pos-attackLog->GetAimedUnitData().pos).size();
		//被弾ユニット→行動ユニットへのルート距離を求める
		const LogElement::UnitLogData operatedUnit=attackLog->GetOperateUnitData();
		const LogElement::UnitLogData aimedUnit=attackLog->GetAimedUnitData();
		const float routeDistance=CalculateRouteDistance(field,stageSize,attackLog->m_unitDataList,operatedUnit,aimedUnit);
		//評価(高い方から判定していく)
		if(routeDistance>=attackLog->GetAimedUnit()->GetMaxMoveDistance() || routeDistance<0.0f){
			//routeDistance<0.0fの時は、到達経路が存在しないということなので、ルート距離が敵の移動距離より長いのと同じ扱いになる。
			evaluate=s_evaluate[3];
		} else if(routeDistance>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength()){
			evaluate=s_evaluate[2];
		} else if(directDistance>=attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()
			|| attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength())
		{
			evaluate=s_evaluate[1];
		} else{
			evaluate=s_evaluate[0];
		}
	}

	return evaluate;
}

std::string ArcherAttackDistance::GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const{
	std::string comment;
	if(mostFrequentEvaluate==SubmissionEvaluation::e_noevaluation){
		//攻撃していない
		comment="もっと射手で攻撃してみよう！";
	} else if(mostFrequentEvaluate==s_evaluate[0]){
		//敵ユニットの攻撃射程内からの攻撃
		comment="敵の射程外から攻撃する事を意識してみると良いよ！";
	} else if(mostFrequentEvaluate==s_evaluate[1]){
		//ルート距離が攻撃射程以下
		comment="弓で攻撃する時は、障害物や味方越しに攻撃してみよう！";
	} else if(mostFrequentEvaluate==s_evaluate[2]){
		//ルート距離が敵の移動距離以下
		comment="敵が次の行動でどこまで行けるかを考えて、敵の攻撃を受けないような位置で攻撃することを意識してみよう！";
	} else if(mostFrequentEvaluate==s_evaluate[3]){
		//これ以上の評価
		comment="言うことなしです！障害物や味方をうまく使って、安全な場所から攻撃できています！";
	}
	return comment;
}

std::string ArcherAttackDistance::GetWholeLookBackActionEmpty()const{
	return "もっと射手で攻撃してみよう！";
}

std::string ArcherAttackDistance::GetSubmissionExplanation()const{
	return "射程の長い射手は、\n地形や味方キャラの壁を利用して安全地帯から攻撃してみよう！";
}

bool ArcherAttackDistance::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_archer);
}

std::string ArcherAttackDistance::GetReason(SubmissionEvaluation rubric)const{
	if(rubric==SubmissionEvaluation::e_noevaluation){
		//描画を行わない
		return "";
	} else if(rubric==s_evaluate[0]){
		return "敵がその場で攻撃できるくらいに近くで攻撃しちゃってるよ！";
	} else if(rubric==s_evaluate[1]){
		return "障害物が周りにないから攻撃した敵の反撃に合いやすそうじゃない？";
	} else if(rubric==s_evaluate[2]){
		return "障害物越しに攻撃できてるけど、案外敵は回り込んで攻撃できそう。";
	} else if(rubric==s_evaluate[3]){
		return "安全地帯からの攻撃、とっても良い感じ！！";
	}
	return "";
}
