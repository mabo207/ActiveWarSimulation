#include"DxLib.h"
#include"HealerPosition.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"WaitLog.h"

int HealerPosition::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- 例外処理
	//	- （なし）
	//- 評価
	//	1.敵物理兵が2体以上、操作ユニットを次の行動で攻撃可能である
	//	2.敵重装兵が1体以上、操作ユニットを次の行動で攻撃可能である
	//	3.敵兵士が1体以上、操作ユニットを次の行動で攻撃可能である
	//	4.敵射手が1体以上、操作ユニットを次の行動で攻撃可能である
	//	5.どの敵物理兵も、操作ユニットを次の行動で攻撃できない
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(evaluateLog);
	const std::vector<LogElement::UnitLogData> *pUnitLogDataList=nullptr;
	std::function<LogElement::UnitLogData()> getAimedUnitData;
	if(attackLog){
		pUnitLogDataList=&attackLog->m_unitDataList;
		getAimedUnitData=[attackLog](){return attackLog->GetOperateUnitData();};
	} else if(waitLog){
		pUnitLogDataList=&waitLog->m_unitDataList;
		getAimedUnitData=[waitLog](){return waitLog->GetOperateUnitData();};
	}
	int evaluate;
	if(pUnitLogDataList==nullptr){
		//ここに来ることはないはずだが、一応エラー処理
		evaluate=-1;
	} else{
		//全ての敵物理アタッカーに対して、攻撃可否判定をし、攻撃可能な魔道士の数を計算
		size_t attackableEnemyArcherCount=0;
		size_t attackableEnemySoldierCount=0;
		size_t attackableEnemyArmerCount=0;
		for(const LogElement::UnitLogData &unitData:*pUnitLogDataList){
			if(unitData.punit->GetBattleStatus().team==Unit::Team::e_enemy){
				//どのパラメータに加算処理をするかを定義する
				std::function<void(void)> addFunc;
				if(unitData.punit->GetBaseStatus().profession==Unit::Profession::e_archer){
					addFunc=[&attackableEnemyArcherCount](){attackableEnemyArcherCount++;};
				} else if(unitData.punit->GetBaseStatus().profession==Unit::Profession::e_soldier){
					addFunc=[&attackableEnemySoldierCount](){attackableEnemySoldierCount++;};
				} else if(unitData.punit->GetBaseStatus().profession==Unit::Profession::e_armer){
					addFunc=[&attackableEnemyArmerCount](){attackableEnemyArmerCount++;};
				} else{
					//加算先がないので、攻撃可否判定をしない
					continue;
				}
				//攻撃可否判定とカウント加算
				if(JudgeAttackable(field,stageSize,*pUnitLogDataList,unitData,getAimedUnitData())){
					addFunc();
				}
			}
		}
		//攻撃可能な敵ユニットの数で評価
		if(attackableEnemyArcherCount+attackableEnemyArmerCount+attackableEnemySoldierCount>=2){
			evaluate=0;
		} else if(attackableEnemyArmerCount==1){
			evaluate=1;
		} else if(attackableEnemySoldierCount==1){
			evaluate=2;
		} else if(attackableEnemyArcherCount==1){
			evaluate=3;
		} else{
			evaluate=4;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> HealerPosition::GetRubricStringInfo(int rubric)const{
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
		//微妙
		rubricStr="OK";
		edgeColor=GetColor(128,196,196);
		break;
	case(3):
		//良い
		rubricStr="Good!!";
		edgeColor=GetColor(196,196,64);
		break;
	case(4):
		//とても良い
		rubricStr="Excellent!!";
		edgeColor=GetColor(196,64,128);
		break;
	}
	return std::make_pair(rubricStr,edgeColor);
}

std::string HealerPosition::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//衛生兵兵がいない
		comment="衛生兵を使ってみよう！";
		break;
	case(0):
		//Bad評価が多い
		comment="衛生兵を動かす時は、敵に攻撃されないような場所に動かすことを意識しよう！";
		break;
	case(1):
		//Not Good評価が多い
		comment="重装兵は攻撃力が高いので、衛生兵を近づかせすぎないように気を付けよう！";
		break;
	case(2):
		//OK評価が多い
		comment="兵士は移動力が高いので、壁役のキャラを前に置いて道を塞ぐように動かすのを意識すると守りやすいぞ！";
		break;
	case(3):
		//Good評価が多い
		comment="射手は攻撃範囲が広く、いつの間に体力を削られてしまうので、できる限り早めに撃破することを心がけよう。";
		break;
	case(4):
		//Excellent評価が多い
		comment="言うことなしです！衛生兵を安全な場所に配置することを意識できています！";
		break;
	}
	return comment;
}

std::string HealerPosition::GetWholeLookBackActionEmpty()const{
	return "もっと衛生兵を動かしてみよう！";
}

std::string HealerPosition::GetSubmissionExplanation()const{
	return "衛生兵は打たれ弱いので、\n敵に攻撃されないように気を付けよう！";
}

bool HealerPosition::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_healer);
}

std::string HealerPosition::GetReason(int rubric)const{
	switch(rubric){
	case(-1):
		//描画を行わない
		return "";
	case(0):
		return "複数の敵が攻撃してきそうだ、危ない！";
	case(1):
		return "攻撃力の高い重装兵に狙われてしまう位置だ。";
	case(2):
		return "機動力のある兵士に目をつけられてしまったようだ……";
	case(3):
		return "射手に狙われている位置だから、油断はできなさそうだ。";
	case(4):
		return "敵が狙いづらい位置に移動できているね！";
	}
	return "";
}