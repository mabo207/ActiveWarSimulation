#include"SubmissionRuleBase.h"
#include"BattleSceneData.h"
#include"Unit.h"
#include"Circle.h"

//-----------------SubmissionRuleBase-------------------
SubmissionEvaluation SubmissionRuleBase::RubricEvaluate(const BattleSceneData * const battleData)const{
	return RubricEvaluate(battleData->m_field,battleData->m_stageSize,battleData->m_scoreObserver->GetLatestLog());
}

float SubmissionRuleBase::CalculateRouteDistance(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const{
	//���O�f�[�^�v�Z�Ǝ��O�f�[�^���p�v�Z�̑g�ݍ��킹
	return CalculateRouteDistance(CalculateInAdvanceRouteData(field,mapSize,unitDataList,operatedUnit,aimedUnit),operatedUnit);
}

float SubmissionRuleBase::CalculateRouteDistance(const std::shared_ptr<InAdvanceRouteData> &inAdvanceData,const LogElement::UnitLogData operatedUnit)const{
	return CalculateRouteDistance(inAdvanceData,operatedUnit.pos);
}

float SubmissionRuleBase::CalculateRouteDistance(const std::shared_ptr<InAdvanceRouteData> &inAdvanceData,const Vector2D from)const{
	//�i�q�_�f�[�^�Ƌ����}�b�v�����ɍ쐬����Ă���̂ŁA�����v�Z��������΂悢
	const std::vector<float> distVec=inAdvanceData->m_latticeField->CalculateRouteDistance(inAdvanceData->m_distanceInfoVec,{from});
	const float routeDistance=distVec.front();//�v�f��1�����Ȃ̂ŁA�擪�v�f�����o���Ηǂ��B
	return routeDistance;
}

std::shared_ptr<SubmissionRuleBase::InAdvanceRouteData> SubmissionRuleBase::CalculateInAdvanceRouteData(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const{
	//�i�q�_�̐N���ۏ��̌v�Z�A���j�b�g�͏��ɉe����^���Ȃ��ݒ�Ȃ̂ŁAbattleData->m_operatedUnit�͒N�ł����Ȃ�
	std::shared_ptr<LatticeBattleField> latticeField=LatticeBattleField::Create(field,mapSize,operatedUnit.punit,false);
	for(const LogElement::UnitLogData &logData:unitDataList){
		//���O�̈ʒu�f�[�^�����ƂɁA���j�b�g�ɂ��i�q�_�N���s����ǉ�(��e���j�b�g����������z�肵�Ă���)
		if(logData.punit!=operatedUnit.punit && logData.punit!=aimedUnit.punit){
			Unit virtualUnit=*logData.punit;
			virtualUnit.Warp(logData.pos);
			latticeField->BecomeImpassibleLattice(&virtualUnit,aimedUnit.punit->GetBattleStatus().team);
		}
	}
	//�����}�b�v�v�Z�BCalculateRouteDistance()�́A��Q�����̍��W���ꂼ��ɑ΂��Ă����܂ł̌o�H������Ԃ��֐��B�����ł�1�_�̂ݕ�����Ηǂ��B
	std::vector<LatticeBattleField::LatticeDistanceInfo> distanceInfoVec;
	latticeField->CalculateLatticeDistanceInfo(distanceInfoVec,aimedUnit.pos);
	//�C���X�^���X�쐬���ĕԂ�
	return std::shared_ptr<InAdvanceRouteData>(new InAdvanceRouteData(latticeField,distanceInfoVec));
}

bool SubmissionRuleBase::JudgeAttackable(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const{
	std::vector<bool> judgeList=JudgeAttackableList(field,mapSize,unitDataList,operatedUnit,{aimedUnit});
	if(judgeList.empty()){
		//�ꉞ��O����
		return false;
	} else{
		return judgeList.front();
	}
}

std::vector<bool> SubmissionRuleBase::JudgeAttackableList(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const std::vector<LogElement::UnitLogData> &aimedUnitList)const{
	//�i�q�_�̐N���ۏ��̌v�Z�A���j�b�g�͏��ɉe����^���Ȃ��ݒ�Ȃ̂ŁAbattleData->m_operatedUnit�͒N�ł����Ȃ�
	std::shared_ptr<LatticeBattleField> latticeField=LatticeBattleField::Create(field,mapSize,operatedUnit.punit,false);
	for(const LogElement::UnitLogData &logData:unitDataList){
		//���j�b�g�ɂ��i�q�_�N���s����ǉ�
		if(logData.punit!=operatedUnit.punit){
			Unit virtualUnit=*logData.punit;
			virtualUnit.Warp(logData.pos);
			latticeField->BecomeImpassibleLattice(&virtualUnit,operatedUnit.punit->GetBattleStatus().team);
		}
	}
	//����������Ōv�Z
	return JudgeAttackableList(latticeField,operatedUnit,aimedUnitList);
}

std::vector<bool> SubmissionRuleBase::JudgeAttackableList(const std::shared_ptr<LatticeBattleField> &latticeField,const LogElement::UnitLogData operatedUnit,const std::vector<LogElement::UnitLogData> &aimedUnitList)const{
	//�GAI�̌v�Z�͓_�����̊֌W��d���̂ŁA�ȈՓI��AI�A���S���Y�����������Ȃ����i�炢�j
	//�����}�b�v�̍쐬
	std::vector<LatticeBattleField::LatticeDistanceInfo> latticeDistanceInfo;
	latticeField->CalculateLatticeDistanceInfo(latticeDistanceInfo,operatedUnit.pos);
	//�U���\�͈͂̈ꗗ���쐬
	const size_t aimedUnitCount=aimedUnitList.size();
	std::vector<std::shared_ptr<Circle>> attackableRangeList;
	attackableRangeList.reserve(aimedUnitCount);
	for(const LogElement::UnitLogData &aimedUnit:aimedUnitList){
		Unit virtualUnit=*aimedUnit.punit;
		virtualUnit.Warp(aimedUnit.pos);
		const Circle *pcircle=dynamic_cast<const Circle *>(virtualUnit.GetUnitCircleShape());//��Ɨp�̃��j�b�g�}�`(����)�̊m��
		if(pcircle!=nullptr){
			attackableRangeList.push_back(std::shared_ptr<Circle>(new Circle(pcircle->GetPosition(),pcircle->GetR()+operatedUnit.punit->GetBattleStatus().weapon->GetLength()+LatticeBattleField::latticeIntervalSize,pcircle->m_fix)));//�U���\�͈�(�������A�ʒu�ɂ���Ă͊i�q�_�����o�ł��Ȃ����߁ALatticeBattleField::latticeIntervalSize�����]���ɍL���͈͂Ŋi�q�_��T��)
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
			retList[aimedIndex]=latticeDistanceInfo[nearestIndexList[aimedIndex]].dist<=operatedUnit.punit->GetMaxMoveDistance();
		}
	}
	return retList;
}
