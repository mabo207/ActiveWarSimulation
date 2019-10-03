#include"SubmissionRuleBase.h"
#include"BattleSceneData.h"
#include"Unit.h"
#include"Circle.h"

//-----------------SubmissionRuleBase-------------------
float SubmissionRuleBase::CalculateRouteDistance(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit *operatedUnit,const Unit *aimedUnit)const{
	//�i�q�_�̐N���ۏ��̌v�Z�A���j�b�g�͏��ɉe����^���Ȃ��ݒ�Ȃ̂ŁAbattleData->m_operatedUnit�͒N�ł����Ȃ�
	std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);
	for(const LogElement::UnitLogData &logData:unitDataList){
		//���j�b�g�ɂ��i�q�_�N���s����ǉ�(��e���j�b�g����������z�肵�Ă���)
		if(logData.punit!=operatedUnit && logData.punit!=aimedUnit){
			latticeField->BecomeImpassibleLattice(logData.punit,aimedUnit->GetBattleStatus().team);
		}
	}
	//�����v�Z�BCalculateRouteDistance()�́A��Q�����̍��W���ꂼ��ɑ΂��Ă����܂ł̌o�H������Ԃ��֐��B�����ł�1�_�̂ݕ�����Ηǂ��B
	const std::vector<float> distVec=latticeField->CalculateRouteDistance(aimedUnit->getPos(),{operatedUnit->getPos()});
	const float routeDistance=distVec.front();//�v�f��1�����Ȃ̂ŁA�擪�v�f�����o���Ηǂ��B
	return routeDistance;
}

bool SubmissionRuleBase::JudgeAttackable(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit *operatedUnit,const Unit *aimedUnit)const{
	//�GAI�̌v�Z�͓_�����̊֌W��d���̂ŁA�ȈՓI��AI�A���S���Y�����������Ȃ����i�炢�j
	//�i�q�_�̐N���ۏ��̌v�Z�A���j�b�g�͏��ɉe����^���Ȃ��ݒ�Ȃ̂ŁAbattleData->m_operatedUnit�͒N�ł����Ȃ�
	std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);
	for(const LogElement::UnitLogData &logData:unitDataList){
		//���j�b�g�ɂ��i�q�_�N���s����ǉ�
		if(logData.punit!=operatedUnit){
			latticeField->BecomeImpassibleLattice(logData.punit,operatedUnit->GetBattleStatus().team);
		}
	}
	//�����}�b�v�̍쐬
	std::vector<LatticeBattleField::LatticeDistanceInfo> latticeDistanceInfo;
	latticeField->CalculateLatticeDistanceInfo(latticeDistanceInfo,operatedUnit->getPos());
	//�U���ۂ̌v�Z
	if(aimedUnit->GetHitJudgeShape()->GetType()==Shape::Type::e_circle){
		const Circle *pcircle=dynamic_cast<const Circle *>(aimedUnit->GetUnitCircleShape());//��Ɨp�̃��j�b�g�}�`(����)�̊m��
		if(pcircle!=nullptr){
			Circle c(pcircle->GetPosition(),pcircle->GetR()+operatedUnit->GetBattleStatus().weapon->GetLength()+LatticeBattleField::latticeIntervalSize,pcircle->m_fix);//�U���\�͈�(�������A�ʒu�ɂ���Ă͊i�q�_�����o�ł��Ȃ����߁ALatticeBattleField::latticeIntervalSize�����]���ɍL���͈͂Ŋi�q�_��T��)
			//�U���\�͈͂̒��ɂ���i�q�_�̒��ōŋߓ_�����߂�
			const size_t size=latticeDistanceInfo.size();
			size_t nearestIndex=size;//���ꂪsize�ł���Ԃ�
			for(size_t i=0;i<size;i++){
				if(latticeField->GetLatticeInShapeAt(i)==LatticeBattleField::LatticePass::e_passable
					&& latticeDistanceInfo[i].dist>=0.0f
					&& c.VJudgePointInsideShape(latticeField->CalculateLatticePointPos(i))
					&& (nearestIndex>=size || latticeDistanceInfo[i].dist<latticeDistanceInfo[nearestIndex].dist))
				{
					//�_i���N���\�����B�\�ŁA�U���\�͈͓��ɂ���A�������݂̍ŋߓ_���߂��̂ł����
					nearestIndex=i;//�ŋߓ_�̍X�V
				}
			}
			if(nearestIndex<size){
				//�ŋߓ_�����݂���Ȃ�΁A���̓_��1��̍s���œ��B�ł��邩�ǂ�����Ԃ�
				return latticeDistanceInfo[nearestIndex].dist<=operatedUnit->GetMaxMoveDistance();
			} else{
				//�ŋߓ_�����݂��Ȃ��Ȃ�΁A���������U���ł��Ȃ��̂�false��Ԃ�
				return false;
			}
		}
	}
	return false;
}
