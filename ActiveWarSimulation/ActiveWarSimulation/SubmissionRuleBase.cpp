#include"SubmissionRuleBase.h"
#include"BattleSceneData.h"
#include"Unit.h"

//-----------------SubmissionRuleBase-------------------
float SubmissionRuleBase::CalculateRouteDistance(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit *operatedUnit,const Unit *aimedUnit)const{
	std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);//格子点の侵入可否情報の計算、ユニットは情報に影響を与えない設定なので、battleData->m_operatedUnitは誰でも問題ない
	for(const LogElement::UnitLogData &logData:unitDataList){
		//ユニットによる格子点侵入不可情報を追加(被弾ユニットが動く事を想定している)
		if(logData.punit!=operatedUnit && logData.punit!=aimedUnit){
			latticeField->BecomeImpassibleLattice(logData.punit,aimedUnit->GetBattleStatus().team);
		}
	}
	const std::vector<float> distVec=latticeField->CalculateRouteDistance(aimedUnit->getPos(),{operatedUnit->getPos()});//CalculateRouteDistance()は、第２引数の座標それぞれに対してそこまでの経路距離を返す関数。ここでは1点のみ分かれば良い
	const float routeDistance=distVec.front();//要素は1つだけなので、先頭要素を取り出せば良い。
	return routeDistance;
}
