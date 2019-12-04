#include"DxLib.h"
#include"MageAttackingOpponent.h"
#include"BattleSceneData.h"
#include"AttackLog.h"

SubmissionEvaluation MageAttackingOpponent::InAdvanceDataEvaluate(const std::shared_ptr<InAdvanceCalculateDataBase> &,const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- 例外処理
	//	- 攻撃しない(-1)
	//- 評価
	//	0. 敵が魔道士・衛生兵である
	//	1. 敵が兵士・射手である
	//	2. 敵が重装兵である
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	SubmissionEvaluation evaluate;
	if(!attackLog){
		//ログがAttackLogでない場合は「攻撃をしなかった」と判断できる
		evaluate=SubmissionEvaluation::e_noevaluation;
	} else{
		//攻撃相手の兵種を見て、評価を行う
		switch(attackLog->GetAimedUnit()->GetBaseStatus().profession){
		case(Unit::Profession::e_healer):
		case(Unit::Profession::e_mage):
			evaluate=SubmissionEvaluation::e_bad;
			break;
		case(Unit::Profession::e_archer):
		case(Unit::Profession::e_soldier):
			evaluate=SubmissionEvaluation::e_ok;
			break;
		case(Unit::Profession::e_armer):
			evaluate=SubmissionEvaluation::e_excellent;
			break;
		}
	}

	return evaluate;
}

std::string MageAttackingOpponent::GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const{
	std::string comment;
	if(mostFrequentEvaluate==SubmissionEvaluation::e_noevaluation){
		//攻撃していない
		comment="もっと魔道士で攻撃してみよう！";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_bad){
		//魔法系ユニットへの攻撃
		comment="魔法攻撃は、魔法を使う敵より武器で戦う敵にした方が効果的だよ！";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_ok){
		//射手・兵士への攻撃
		comment="できれば、武器を使う敵の中でも特に魔法に弱い重装兵をもっと狙ってみよう！";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_excellent){
		//重装兵への攻撃
		comment="言うことなしです！魔法に弱い敵を狙って攻撃できています！";
	}
	return comment;
}

std::string MageAttackingOpponent::GetWholeLookBackActionEmpty()const{
	return "もっと魔道士で攻撃してみよう！";
}

std::string MageAttackingOpponent::GetSubmissionExplanation()const{
	return "魔道士での攻撃の際は、\n魔法に弱い敵を狙う事を心がけよう！";
}

bool MageAttackingOpponent::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_mage);
}

std::string MageAttackingOpponent::GetReason(SubmissionEvaluation rubric)const{
	if(rubric==SubmissionEvaluation::e_noevaluation){
		//描画を行わない
		return "";
	} else if(rubric==SubmissionEvaluation::e_bad){
		return "魔道士や衛生兵には魔法攻撃は効果が薄いみたいだ。";
	} else if(rubric==SubmissionEvaluation::e_ok){
		return "兵士や射手を魔法で攻撃するのは、まあまあ効果的だ。";
	} else if(rubric==SubmissionEvaluation::e_excellent){
		return "魔法が弱点の重装兵を攻撃できています！";
	}
	return "";
}
