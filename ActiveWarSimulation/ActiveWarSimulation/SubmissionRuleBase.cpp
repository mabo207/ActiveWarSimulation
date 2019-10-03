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
	std::vector<bool> judgeList=JudgeAttackableList(battleData,unitDataList,operatedUnit,{aimedUnit});
	if(judgeList.empty()){
		//�ꉞ��O����
		return false;
	} else{
		return judgeList.front();
	}
}

std::vector<bool> SubmissionRuleBase::JudgeAttackableList(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit * operatedUnit,const std::vector<const Unit *> &aimedUnitList)const{
	//�i�q�_�̐N���ۏ��̌v�Z�A���j�b�g�͏��ɉe����^���Ȃ��ݒ�Ȃ̂ŁAbattleData->m_operatedUnit�͒N�ł����Ȃ�
	std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);
	for(const LogElement::UnitLogData &logData:unitDataList){
		//���j�b�g�ɂ��i�q�_�N���s����ǉ�
		if(logData.punit!=operatedUnit){
			latticeField->BecomeImpassibleLattice(logData.punit,operatedUnit->GetBattleStatus().team);
		}
	}
	//����������Ōv�Z
	return JudgeAttackableList(latticeField,operatedUnit,aimedUnitList);
}

std::vector<bool> SubmissionRuleBase::JudgeAttackableList(const std::shared_ptr<LatticeBattleField> &latticeField,const Unit * operatedUnit,const std::vector<const Unit *> &aimedUnitList)const{
	//�GAI�̌v�Z�͓_�����̊֌W��d���̂ŁA�ȈՓI��AI�A���S���Y�����������Ȃ����i�炢�j
	//�����}�b�v�̍쐬
	std::vector<LatticeBattleField::LatticeDistanceInfo> latticeDistanceInfo;
	latticeField->CalculateLatticeDistanceInfo(latticeDistanceInfo,operatedUnit->getPos());
	//�U���\�͈͂̈ꗗ���쐬
	const size_t aimedUnitCount=aimedUnitList.size();
	std::vector<std::shared_ptr<Circle>> attackableRangeList;
	attackableRangeList.reserve(aimedUnitCount);
	for(const Unit *aimedUnit:aimedUnitList){
		const Circle *pcircle=dynamic_cast<const Circle *>(aimedUnit->GetUnitCircleShape());//��Ɨp�̃��j�b�g�}�`(����)�̊m��
		if(pcircle!=nullptr){
			attackableRangeList.push_back(std::shared_ptr<Circle>(new Circle(pcircle->GetPosition(),pcircle->GetR()+operatedUnit->GetBattleStatus().weapon->GetLength()+LatticeBattleField::latticeIntervalSize,pcircle->m_fix)));//�U���\�͈�(�������A�ʒu�ɂ���Ă͊i�q�_�����o�ł��Ȃ����߁ALatticeBattleField::latticeIntervalSize�����]���ɍL���͈͂Ŋi�q�_��T��)
		} else{
			attackableRangeList.push_back(std::shared_ptr<Circle>(nullptr));//�X�J�����Ă���
		}
	}
	//�U���\�͈͂̒��ɂ���i�q�_�̒��ōŋߓ_�����߂�
	const size_t size=latticeDistanceInfo.size();
	std::vector<size_t> nearestIndexList(aimedUnitCount,size);
	for(size_t i=0;i<size;i++){
		//�ǂ̃��j�b�g���U�����邩�Ɋւ�炸�s������
		if(latticeField->GetLatticeInShapeAt(i)==LatticeBattleField::LatticePass::e_passable
			&& latticeDistanceInfo[i].dist>=0.0f)
		{
			for(size_t aimedIndex=0;aimedIndex<aimedUnitCount;aimedIndex++){
				//�U���Ώۂɂ���Ĕ�����@���ς�����
				if((attackableRangeList[aimedIndex] && attackableRangeList[aimedIndex]->VJudgePointInsideShape(latticeField->CalculateLatticePointPos(i)))
					&& (nearestIndexList[aimedIndex]>=size || latticeDistanceInfo[i].dist<latticeDistanceInfo[nearestIndexList[aimedIndex]].dist))
				{
					//�_i���N���\�����B�\�ŁA�U���\�͈͓��ɂ���A�������݂̍ŋߓ_���߂��̂ł����
					nearestIndexList[aimedIndex]=i;//�ŋߓ_�̍X�V
				}
			}
		}
	}
	//���̎�Ԃł̍U���ۂ��e���j�b�g�ɑ΂��ċ��߂�
	std::vector<bool> retList(aimedUnitCount,false);
	for(size_t aimedIndex=0;aimedIndex<aimedUnitCount;aimedIndex++){
		if(nearestIndexList[aimedIndex]<size){
			//�ŋߓ_�����݂���Ȃ�΁A���̓_��1��̍s���œ��B�ł��邩�ǂ�����Ԃ�
			retList[aimedIndex]=latticeDistanceInfo[nearestIndexList[aimedIndex]].dist<=operatedUnit->GetMaxMoveDistance();
		}
	}
	return retList;
}
