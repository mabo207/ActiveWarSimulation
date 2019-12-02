#include"DxLib.h"
#include"ArmerPosition.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"WaitLog.h"

SubmissionEvaluation ArmerPosition::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- 例外処理
	//	- （なし）
	//- 評価
	//	1. 敵魔道士が2体以上、操作ユニットを次の行動で攻撃可能である
	//	2. 敵魔道士が1体以上、操作ユニットを次の行動で攻撃可能である
	//	3. どの敵魔道士も、操作ユニットを次の行動で攻撃できない
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(evaluateLog);
	const std::vector<LogElement::UnitLogData> *pUnitLogDataList=nullptr;
	std::function<LogElement::UnitLogData()> getAimedUnitFunc;
	if(attackLog){
		pUnitLogDataList=&attackLog->m_unitDataList;
		getAimedUnitFunc=[attackLog](){return attackLog->GetOperateUnitData();};
	} else if(waitLog){
		pUnitLogDataList=&waitLog->m_unitDataList;
		getAimedUnitFunc=[waitLog](){return waitLog->GetOperateUnitData();};
	}
	SubmissionEvaluation evaluate;
	if(pUnitLogDataList==nullptr){
		//ここに来ることはないはずだが、一応エラー処理
		evaluate=SubmissionEvaluation::e_noevaluation;
	} else{
		//全ての敵魔道士に対して、攻撃可否判定をし、攻撃可能な魔道士の数を計算
		size_t attackableEnemyMageCount=0;
		for(const LogElement::UnitLogData &unitData:*pUnitLogDataList){
			if(unitData.punit->GetBaseStatus().profession==Unit::Profession::e_mage && unitData.punit->GetBattleStatus().team==Unit::Team::e_enemy){
				if(JudgeAttackable(field,stageSize,*pUnitLogDataList,unitData,getAimedUnitFunc())){
					attackableEnemyMageCount++;
				}
			}
		}
		//攻撃可能な魔道士の数で評価
		if(attackableEnemyMageCount==0){
			evaluate=SubmissionEvaluation::e_excellent;
		} else if(attackableEnemyMageCount==1){
			evaluate=SubmissionEvaluation::e_ok;
		} else{
			evaluate=SubmissionEvaluation::e_bad;
		}
	}

	return evaluate;
}

std::string ArmerPosition::GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const{
	std::string comment;
	if(mostFrequentEvaluate==SubmissionEvaluation::e_noevaluation){
		//重装兵がいない
		comment="重装兵を使ってみよう！";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_bad){
		//Bad評価が多い
		comment="重装兵を動かす時は、敵の魔道士に近づけすぎないように気を付けよう！";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_ok){
		//Not Good評価が多い
		comment="敵の魔道士が重装兵に近づいてきたら、優先的に倒してみるのも良いよ！";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_excellent){
		//これ以上の評価
		comment="言うことなしです！魔法使いが重装兵に攻撃するのをできる限り防げています！";
	}
	return comment;
}

std::string ArmerPosition::GetWholeLookBackActionEmpty()const{
	return "もっと重装兵を動かしてみよう！";
}

std::string ArmerPosition::GetSubmissionExplanation()const{
	return "重装兵は魔法攻撃に弱いので、\n魔道士に攻撃されないように気を付けよう！";
}

bool ArmerPosition::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_armer);
}

std::string ArmerPosition::GetReason(SubmissionEvaluation rubric)const{
	if(rubric==SubmissionEvaluation::e_noevaluation){
		//描画を行わない
		return "";
	} else if(rubric==SubmissionEvaluation::e_bad){
		return "近くに魔道士が複数いて危ない位置だ！";
	} else if(rubric==SubmissionEvaluation::e_ok){
		return "次の手番で自分を攻撃できそうな敵魔道士がいるみたいだ……";
	} else if(rubric==SubmissionEvaluation::e_excellent){
		return "魔道士は攻撃できる位置にはいないみたい！";
	}
	return "";
}
