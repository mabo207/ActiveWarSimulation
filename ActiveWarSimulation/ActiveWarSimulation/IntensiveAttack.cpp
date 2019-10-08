#include"DxLib.h"
#include"IntensiveAttack.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include<algorithm>

int IntensiveAttack::RubricEvaluate(const BattleSceneData * const battleData)const{
	//- 例外処理
	//	- 攻撃しない(-1)
	//	- 撃破時は必ず最大評価(4)
	//	- 残り1体の時は撃破時以外は評価しない（-1）
	//	- 残り2体の時は最高評価でない時に評価2とする。(2)
	//- 評価
	//	1. 攻撃相手よりHP割合の低いユニットが3体以上いる
	//	2. 攻撃相手よりHP割合の低いユニットが2体いる
	//	3. 攻撃相手よりHP割合の低いユニットが1体いる
	//	4. 攻撃相手よりHP割合の低いユニットがいない
	//懸念点として、「そもそもの問題として、excellent評価が撃破時と初回攻撃時に出てしまうから下手でも最高評価をたたき出せてしまうのでは」説がある。
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(battleData->m_scoreObserver->GetLatestLog());
	int evaluate;
	const int maxEvaluate=3;
	if(!attackLog){
		//ログがAttackLogでない場合は「攻撃をしなかった」と判断できる
		evaluate=-1;
	} else{
		//撃破できるかの判定
		const LogElement::UnitLogData aimedData=attackLog->GetAimedUnitData();
		//全ての敵のHP割合を出す
		std::vector<std::pair<const Unit *,int>> hpRateVec;
		for(const LogElement::UnitLogData &unitData:attackLog->m_unitDataList){
			if(unitData.punit->GetBattleStatus().team==Unit::Team::e_enemy){
				//全ての敵について格納
				hpRateVec.push_back(std::make_pair(unitData.punit,32768*unitData.hp/unitData.punit->GetBaseStatus().maxHP));//intで計算するので切り捨てが怖い、ある程度大きい数値をかけることで割合のソートが可能になる。
			}
		}
		//HP割合の低い順にソートする
		const auto sortFunc=[](const std::pair<const Unit *,int> left,const std::pair<const Unit *,int> right){
			if(left.second<right.second){
				return true;
			} else if(left.second>right.second){
				return false;
			}
			return left.first<right.first;
		};
		std::sort(hpRateVec.begin(),hpRateVec.end(),sortFunc);
		//敵の数を記録(評価に用いる)
		const size_t enemyNum=hpRateVec.size();
		//攻撃相手が何番目だったか測定、同率割合に注意
		size_t order=0;//順位（自分よりHP割合の小さいキャラが何体いるか）を記録、同率を考慮する
		for(size_t i=0;i<enemyNum;i++){
			if(hpRateVec[i].first==aimedData.punit){
				//攻撃相手が見つかった
				break;
			} else{
				//攻撃相手が見つからなかった場合、順位更新する
				if(i+1<enemyNum && hpRateVec[i].second!=hpRateVec[i+1].second){
					//[i]のHP割合が[i+1]のHP割合に等しくないなら、[0]~[i]の要素は攻撃相手より順位が上
					order=i+1;//順位更新([0]から数えるので+1する)
				}
			}
		}
		//順番と敵数に基づき評価
		if(aimedData.hp<=attackLog->GetAttackInfo().damage){
			//撃破していた場合は、残りHP割合の順序に関わらず最大評価を与える
			evaluate=maxEvaluate;
		} else if(enemyNum==1){
			//残り1体の場合は、撃破時以外評価なし
			evaluate=-1;
		} else if(enemyNum==2 && order>0){
			//残り2体の場合は、最大評価でない方の評価を通常と変える
			evaluate=1;
		} else if(order==0){
			//攻撃相手よりHP割合の高いキャラがいない
			evaluate=maxEvaluate;
		} else if(order==1){
			//攻撃相手よりHP割合の高いキャラが1体いる
			evaluate=2;
		} else if(order==2){
			//2体いる
			evaluate=1;
		} else{
			//3体以上
			evaluate=0;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> IntensiveAttack::GetRubricStringInfo(int rubric)const{
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
		//良い
		rubricStr="Good!!";
		edgeColor=GetColor(196,196,64);
		break;
	case(3):
		//とても良い
		rubricStr="Excellent!!";
		edgeColor=GetColor(196,64,128);
		break;
	}
	return std::make_pair(rubricStr,edgeColor);
}

std::string IntensiveAttack::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//攻撃していない
		comment="まずは積極的に攻撃してみよう！";
		break;
	case(0):
		//HP割合が高いユニットを狙っていた(3人以上)
		comment="バラバラに敵を攻撃するのではなく、１体の敵を集中的に攻撃した方が自軍を守るためにも効果的ですよ！";
		break;
	case(1):
		//HP割合が高いユニットを狙っていた(2人以上)
		comment="集中攻撃をしやすくするために、攻撃射程の短いキャラは前、射程の長いキャラは後ろに配置することを心がけてみよう！";
		break;
	case(2):
		//HP割合が高いユニットを狙っていた(1人以上)
		comment="集中攻撃をよく心がけていますが、弱っている敵を見落としているかもしれません。";
		break;
	case(3):
		//集中攻撃できている
		comment="言うことなしです！集中攻撃で敵の戦力を削る事ができています！";
		break;
	}
	return comment;
}

std::string IntensiveAttack::GetWholeLookBackActionEmpty()const{
	return "積極的に攻撃してみよう！";
}

std::string IntensiveAttack::GetSubmissionExplanation()const{
	return "攻撃は最大の防御！\n同じ敵を集中攻撃し素早く敵戦力を削ろう！";
}

bool IntensiveAttack::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession!=Unit::Profession::e_healer);
}

std::string IntensiveAttack::GetReason(int rubric)const{
	switch(rubric){
	case(-1):
		//描画を行わない
		return "";
	case(0):
		return "残りHPが少ない敵がほとんどだ、特定の敵を集中攻撃した方が良いんじゃない？";
	case(1):
		return "別々の敵を狙っていないか？集中攻撃が大事だ。";
	case(2):
		return "もっと手頃に倒せる敵がいるかもしれない、探してみよう。";
	case(3):
		return "残りHPが少ない敵を集中して攻撃できています！";
	}
	return "";
}
