#include"DxLib.h"
#include"ArmerPosition.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"WaitLog.h"

int ArmerPosition::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
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
	int evaluate;
	if(pUnitLogDataList==nullptr){
		//ここに来ることはないはずだが、一応エラー処理
		evaluate=-1;
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
			evaluate=2;
		} else if(attackableEnemyMageCount==1){
			evaluate=1;
		} else{
			evaluate=0;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> ArmerPosition::GetRubricStringInfo(int rubric)const{
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
		//微妙
		rubricStr="Not good";
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

std::string ArmerPosition::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//重装兵がいない
		comment="重装兵を使ってみよう！";
		break;
	case(0):
		//Bad評価が多い
		comment="重装兵を動かす時は、敵の魔道士に近づけすぎないように気を付けよう！";
		break;
	case(1):
		//Not Good評価が多い
		comment="敵の魔道士が重装兵に近づいてきたら、優先的に倒してみるのも良いよ！";
		break;
	case(2):
		//これ以上の評価
		comment="言うことなしです！魔法使いが重装兵に攻撃するのをできる限り防げています！";
		break;
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

std::string ArmerPosition::GetReason(int rubric)const{
	switch(rubric){
	case(-1):
		//描画を行わない
		return "";
	case(0):
		return "近くに魔道士が複数いて危ない位置だ！";
	case(1):
		return "次の手番で自分を攻撃できそうな敵魔道士がいるみたいだ……";
	case(2):
		return "魔道士は攻撃できる位置にはいないみたい！";
	}
	return "";
}
