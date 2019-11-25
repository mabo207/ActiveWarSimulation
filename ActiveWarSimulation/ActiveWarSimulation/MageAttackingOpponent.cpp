#include"DxLib.h"
#include"MageAttackingOpponent.h"
#include"BattleSceneData.h"
#include"AttackLog.h"

int MageAttackingOpponent::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- 例外処理
	//	- 攻撃しない(-1)
	//- 評価
	//	0. 敵が魔道士・衛生兵である
	//	1. 敵が兵士・射手である
	//	2. 敵が重装兵である
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	int evaluate;
	if(!attackLog){
		//ログがAttackLogでない場合は「攻撃をしなかった」と判断できる
		evaluate=-1;
	} else{
		//攻撃相手の兵種を見て、評価を行う
		switch(attackLog->GetAimedUnit()->GetBaseStatus().profession){
		case(Unit::Profession::e_healer):
		case(Unit::Profession::e_mage):
			evaluate=0;
			break;
		case(Unit::Profession::e_archer):
		case(Unit::Profession::e_soldier):
			evaluate=1;
			break;
		case(Unit::Profession::e_armer):
			evaluate=2;
			break;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> MageAttackingOpponent::GetRubricStringInfo(int rubric)const{
	std::string rubricStr;
	unsigned int edgeColor;
	switch(rubric){
	case(-1):
		rubricStr="";
		edgeColor=GetColor(0,0,0);
		break;
	case(0):
		//悪い
		rubricStr="Bad";
		edgeColor=GetColor(96,96,196);
		break;
	case(1):
		//まあまあ
		rubricStr="OK";
		edgeColor=GetColor(128,128,196);
		break;
	case(2):
		//完璧
		rubricStr="Good!!";
		edgeColor=GetColor(196,196,64);
		break;
	}
	return std::make_pair(rubricStr,edgeColor);
}

std::string MageAttackingOpponent::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//攻撃していない
		comment="もっと魔道士で攻撃してみよう！";
		break;
	case(0):
		//魔法系ユニットへの攻撃
		comment="魔法攻撃は、魔法を使う敵より武器で戦う敵にした方が効果的だよ！";
		break;
	case(1):
		//射手・兵士への攻撃
		comment="できれば、武器を使う敵の中でも特に魔法に弱い重装兵をもっと狙ってみよう！";
		break;
	case(2):
		//重装兵への攻撃
		comment="言うことなしです！魔法に弱い敵を狙って攻撃できています！";
		break;
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

std::string MageAttackingOpponent::GetReason(int rubric)const{
	switch(rubric){
	case(-1):
		//描画を行わない
		return "";
	case(0):
		return "魔道士や衛生兵には魔法攻撃は効果が薄いみたいだ。";
	case(1):
		return "兵士や射手を魔法で攻撃するのは、まあまあ効果的だ。";
	case(2):
		return "魔法が弱点の重装兵を攻撃できています！";
	}
	return "";
}
