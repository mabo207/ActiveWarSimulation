#include"SubmissionRuleBase.h"
#include"BattleSceneData.h"
#include"Unit.h"
#include"Circle.h"

//-----------------SubmissionRuleBase-------------------
float SubmissionRuleBase::CalculateRouteDistance(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit *operatedUnit,const Unit *aimedUnit)const{
	//格子点の侵入可否情報の計算、ユニットは情報に影響を与えない設定なので、battleData->m_operatedUnitは誰でも問題ない
	std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);
	for(const LogElement::UnitLogData &logData:unitDataList){
		//ユニットによる格子点侵入不可情報を追加(被弾ユニットが動く事を想定している)
		if(logData.punit!=operatedUnit && logData.punit!=aimedUnit){
			latticeField->BecomeImpassibleLattice(logData.punit,aimedUnit->GetBattleStatus().team);
		}
	}
	//距離計算。CalculateRouteDistance()は、第２引数の座標それぞれに対してそこまでの経路距離を返す関数。ここでは1点のみ分かれば良い。
	const std::vector<float> distVec=latticeField->CalculateRouteDistance(aimedUnit->getPos(),{operatedUnit->getPos()});
	const float routeDistance=distVec.front();//要素は1つだけなので、先頭要素を取り出せば良い。
	return routeDistance;
}

bool SubmissionRuleBase::JudgeAttackable(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit *operatedUnit,const Unit *aimedUnit)const{
	std::vector<bool> judgeList=JudgeAttackableList(battleData,unitDataList,operatedUnit,{aimedUnit});
	if(judgeList.empty()){
		//一応例外処理
		return false;
	} else{
		return judgeList.front();
	}
}

std::vector<bool> SubmissionRuleBase::JudgeAttackableList(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit * operatedUnit,const std::vector<const Unit *> &aimedUnitList)const{
	//格子点の侵入可否情報の計算、ユニットは情報に影響を与えない設定なので、battleData->m_operatedUnitは誰でも問題ない
	std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);
	for(const LogElement::UnitLogData &logData:unitDataList){
		//ユニットによる格子点侵入不可情報を追加
		if(logData.punit!=operatedUnit){
			latticeField->BecomeImpassibleLattice(logData.punit,operatedUnit->GetBattleStatus().team);
		}
	}
	//分離したやつで計算
	return JudgeAttackableList(latticeField,operatedUnit,aimedUnitList);
}

std::vector<bool> SubmissionRuleBase::JudgeAttackableList(const std::shared_ptr<LatticeBattleField> &latticeField,const Unit * operatedUnit,const std::vector<const Unit *> &aimedUnitList)const{
	//敵AIの計算は点数化の関係上重いので、簡易的なAIアルゴリズムを実装しなおす（つらい）
	//距離マップの作成
	std::vector<LatticeBattleField::LatticeDistanceInfo> latticeDistanceInfo;
	latticeField->CalculateLatticeDistanceInfo(latticeDistanceInfo,operatedUnit->getPos());
	//攻撃可能範囲の一覧を作成
	const size_t aimedUnitCount=aimedUnitList.size();
	std::vector<std::shared_ptr<Circle>> attackableRangeList;
	attackableRangeList.reserve(aimedUnitCount);
	for(const Unit *aimedUnit:aimedUnitList){
		const Circle *pcircle=dynamic_cast<const Circle *>(aimedUnit->GetUnitCircleShape());//作業用のユニット図形(内部)の確保
		if(pcircle!=nullptr){
			attackableRangeList.push_back(std::shared_ptr<Circle>(new Circle(pcircle->GetPosition(),pcircle->GetR()+operatedUnit->GetBattleStatus().weapon->GetLength()+LatticeBattleField::latticeIntervalSize,pcircle->m_fix)));//攻撃可能範囲(ただし、位置によっては格子点を検出できないため、LatticeBattleField::latticeIntervalSizeだけ余分に広い範囲で格子点を探す)
		} else{
			attackableRangeList.push_back(std::shared_ptr<Circle>(nullptr));//スカを入れておく
		}
	}
	//攻撃可能範囲の中にある格子点の中で最近点を求める
	const size_t size=latticeDistanceInfo.size();
	std::vector<size_t> nearestIndexList(aimedUnitCount,size);
	for(size_t i=0;i<size;i++){
		//どのユニットを攻撃するかに関わらず行う判定
		if(latticeField->GetLatticeInShapeAt(i)==LatticeBattleField::LatticePass::e_passable
			&& latticeDistanceInfo[i].dist>=0.0f)
		{
			for(size_t aimedIndex=0;aimedIndex<aimedUnitCount;aimedIndex++){
				//攻撃対象によって判定方法が変わるもの
				if((attackableRangeList[aimedIndex] && attackableRangeList[aimedIndex]->VJudgePointInsideShape(latticeField->CalculateLatticePointPos(i)))
					&& (nearestIndexList[aimedIndex]>=size || latticeDistanceInfo[i].dist<latticeDistanceInfo[nearestIndexList[aimedIndex]].dist))
				{
					//点iが侵入可能かつ到達可能で、攻撃可能範囲内にあり、尚且つ現在の最近点より近いのであれば
					nearestIndexList[aimedIndex]=i;//最近点の更新
				}
			}
		}
	}
	//次の手番での攻撃可否を各ユニットに対して求める
	std::vector<bool> retList(aimedUnitCount,false);
	for(size_t aimedIndex=0;aimedIndex<aimedUnitCount;aimedIndex++){
		if(nearestIndexList[aimedIndex]<size){
			//最近点が存在するならば、その点に1回の行動で到達できるかどうかを返す
			retList[aimedIndex]=latticeDistanceInfo[nearestIndexList[aimedIndex]].dist<=operatedUnit->GetMaxMoveDistance();
		}
	}
	return retList;
}
