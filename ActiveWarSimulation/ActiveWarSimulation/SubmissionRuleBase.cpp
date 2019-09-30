#include"SubmissionRuleBase.h"
#include"BattleSceneData.h"
#include"Unit.h"

//-----------------SubmissionRuleBase-------------------
float SubmissionRuleBase::CalculateRouteDistance(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit *operatedUnit,const Unit *aimedUnit)const{
	std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);//�i�q�_�̐N���ۏ��̌v�Z�A���j�b�g�͏��ɉe����^���Ȃ��ݒ�Ȃ̂ŁAbattleData->m_operatedUnit�͒N�ł����Ȃ�
	for(const LogElement::UnitLogData &logData:unitDataList){
		//���j�b�g�ɂ��i�q�_�N���s����ǉ�(��e���j�b�g����������z�肵�Ă���)
		if(logData.punit!=operatedUnit && logData.punit!=aimedUnit){
			latticeField->BecomeImpassibleLattice(logData.punit,aimedUnit->GetBattleStatus().team);
		}
	}
	const std::vector<float> distVec=latticeField->CalculateRouteDistance(aimedUnit->getPos(),{operatedUnit->getPos()});//CalculateRouteDistance()�́A��Q�����̍��W���ꂼ��ɑ΂��Ă����܂ł̌o�H������Ԃ��֐��B�����ł�1�_�̂ݕ�����Ηǂ�
	const float routeDistance=distVec.front();//�v�f��1�����Ȃ̂ŁA�擪�v�f�����o���Ηǂ��B
	return routeDistance;
}
