#include"DxLib.h"
#include"ProtectFriend.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"WaitLog.h"

int ProtectFriend::RubricEvaluate(const BattleSceneData * const battleData)const{
	//- 評価に用いるデータ
	//	- いずれかの敵ユニットの次の行動で攻撃可能となる味方後衛ユニットの数(a)
	//	- 操作ユニットをマップから消した時の上記のデータ(b)
	//- 例外処理
	//	- b==0
	//- 評価
	//	1. (b-a)/b==0(誰も守れていない)
	//	2. (b-a)/b<1(誰か守れている)
	//	3. (b-a)/b==1(全ての味方を守れている)
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(battleData->m_scoreObserver->GetLatestLog());
	const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(battleData->m_scoreObserver->GetLatestLog());
	const std::vector<LogElement::UnitLogData> *pUnitLogDataList=nullptr;
	std::function<LogElement::UnitLogData()> getOperatedUnitData;
	if(attackLog){
		pUnitLogDataList=&attackLog->m_unitDataList;
		getOperatedUnitData=[attackLog](){return attackLog->GetOperateUnitData();};
	} else if(waitLog){
		pUnitLogDataList=&waitLog->m_unitDataList;
		getOperatedUnitData=[waitLog](){return waitLog->GetOperateUnitData();};
	}
	int evaluate;
	if(pUnitLogDataList==nullptr){
		//ここに来ることはないはずだが、一応エラー処理
		evaluate=-1;
	} else{
		//全ての敵に対して、味方後衛ユニットへの攻撃可否判定をする
		LogElement::UnitLogData operatedUnit=getOperatedUnitData();
		//後衛ユニット一覧の構築
		std::vector<LogElement::UnitLogData> friendBackUnit;
		for(const LogElement::UnitLogData &logData:*pUnitLogDataList){
			if(logData.punit->GetBattleStatus().team==Unit::Team::e_player){
				switch(logData.punit->GetBaseStatus().profession){
				case(Unit::Profession::e_archer):
				case(Unit::Profession::e_mage):
				case(Unit::Profession::e_healer):
					friendBackUnit.push_back(logData);
					break;
				}
			}
		}
		const size_t friendBackUnitCount=friendBackUnit.size();
		//攻撃可否の計算
		std::vector<std::vector<std::pair<bool,bool>>> judgeMatrix;//judgeMatrix[i][j]:ある敵ユニット[i]の味方後衛ユニット[j]への攻撃可否。firstがnotExist,secondがexistに対応
		for(const LogElement::UnitLogData &attackerData:*pUnitLogDataList){
			//敵の攻撃できるキャラ全てについて探索する
			if(attackerData.punit->GetBattleStatus().team==Unit::Team::e_enemy && attackerData.punit->GetBaseStatus().profession!=Unit::Profession::e_healer){
				//行動できる範囲を求める
				std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);
				for(const LogElement::UnitLogData &logData:*pUnitLogDataList){
					//ユニットによる格子点侵入不可情報を追加、ただし操作していたユニットは除く
					if(logData.punit!=attackerData.punit && logData.punit!=operatedUnit.punit){
						latticeField->BecomeImpassibleLattice(logData.punit,attackerData.punit->GetBattleStatus().team);
					}
				}
				//操作ユニットがいない時の後衛ユニットの被弾リスト作成
				const std::vector<bool> notExistJudge=JudgeAttackableList(latticeField,attackerData,friendBackUnit);
				//行動範囲に操作ユニットの影響を与える
				Unit virtualUnit=*operatedUnit.punit;
				virtualUnit.Warp(operatedUnit.pos);
				latticeField->BecomeImpassibleLattice(&virtualUnit,attackerData.punit->GetBattleStatus().team);
				//操作ユニットがいる時の後衛ユニットの被弾リスト作成
				const std::vector<bool> existJudge=JudgeAttackableList(latticeField,attackerData,friendBackUnit);
				//judgeMatrixへの格納
				std::vector<std::pair<bool,bool>> attackerJudgeList(friendBackUnitCount,std::make_pair(false,false));
				for(size_t i=0;i<friendBackUnitCount;i++){
					attackerJudgeList[i]=std::make_pair(notExistJudge[i],existJudge[i]);
				}
				judgeMatrix.push_back(attackerJudgeList);
			}
		}
		//割合計算
		int notExistCount=0,existCount=0;//自分がいない時・いる時それぞれの攻撃される可能性のある後衛ユニットの数
		for(size_t j=0;j<friendBackUnitCount;j++){
			//味方後衛ユニット[j]に対して攻撃可能な敵ユニットが存在するかどうかを計算
			bool notExistFlag=false,existFlag=false;
			for(size_t i=0,attackerCount=judgeMatrix.size();i<attackerCount;i++){
				notExistFlag=notExistFlag | judgeMatrix[i][j].first;
				existFlag=existFlag | judgeMatrix[i][j].second;
			}
			//加算
			if(notExistFlag){
				notExistCount++;
			}
			if(existFlag){
				existCount++;
			}
		}
		//a,bの割合で評価
		if(notExistCount==0){
			//操作ユニットがいなくても誰も攻撃されない時
			evaluate=-1;
		} else if(existCount==notExistCount){
			//操作ユニットがいても攻撃可否関係が変化しない時
			evaluate=0;
		} else if(existCount>0){
			//操作ユニットが一部の攻撃を防いでいる時
			evaluate=1;
		} else if(existCount==0){
			//操作ユニットが全ての攻撃を防いでいる時
			evaluate=2;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> ProtectFriend::GetRubricStringInfo(int rubric)const{
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

std::string ProtectFriend::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//兵士や重装兵がいない
		comment="兵士や重装兵を使ってみよう！";
		break;
	case(0):
		//Bad評価が多い
		comment="後衛ユニットと敵ユニットの間に前衛ユニットを配置することで後衛ユニットを守ってみよう！";
		break;
	case(1):
		//Not Good評価が多い
		comment="道幅が比較的狭い場所に前衛ユニットを配置して、後衛ユニットをしっかり守ってあげよう！";
		break;
	case(2):
		//これ以上の評価
		comment="言うことなしです！前衛ユニットで後衛ユニットをしっかり守れています！";
		break;
	}
	return comment;
}

std::string ProtectFriend::GetWholeLookBackActionEmpty()const{
	return "後衛ユニットが前衛ユニットから離れすぎて、適切なサポートがしにくくなっていませんか？";
}

std::string ProtectFriend::GetSubmissionExplanation()const{
	return "射手・魔道士・衛生兵といった後衛ユニットは打たれ弱いので、\n兵士・重装兵といった前衛ユニットを壁にして守ってあげよう！";
}

bool ProtectFriend::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& (battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_armer
			|| battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_soldier));
}

