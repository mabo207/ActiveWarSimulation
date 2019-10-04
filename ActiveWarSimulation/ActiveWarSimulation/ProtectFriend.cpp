#include"DxLib.h"
#include"ProtectFriend.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"WaitLog.h"

int ProtectFriend::RubricEvaluate(const BattleSceneData * const battleData)const{
	//- �]���ɗp����f�[�^
	//	- �����ꂩ�̓G���j�b�g�̎��̍s���ōU���\�ƂȂ閡����q���j�b�g�̐�(a)
	//	- ���샆�j�b�g���}�b�v������������̏�L�̃f�[�^(b)
	//- ��O����
	//	- b==0
	//- �]��
	//	1. (b-a)/b==0(�N�����Ă��Ȃ�)
	//	2. (b-a)/b<1(�N�����Ă���)
	//	3. (b-a)/b==1(�S�Ă̖��������Ă���)
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
		//�����ɗ��邱�Ƃ͂Ȃ��͂������A�ꉞ�G���[����
		evaluate=-1;
	} else{
		//�S�Ă̓G�ɑ΂��āA������q���j�b�g�ւ̍U���۔��������
		LogElement::UnitLogData operatedUnit=getOperatedUnitData();
		//��q���j�b�g�ꗗ�̍\�z
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
		//�U���ۂ̌v�Z
		std::vector<std::vector<std::pair<bool,bool>>> judgeMatrix;//judgeMatrix[i][j]:����G���j�b�g[i]�̖�����q���j�b�g[j]�ւ̍U���ہBfirst��notExist,second��exist�ɑΉ�
		for(const LogElement::UnitLogData &attackerData:*pUnitLogDataList){
			//�G�̍U���ł���L�����S�Ăɂ��ĒT������
			if(attackerData.punit->GetBattleStatus().team==Unit::Team::e_enemy && attackerData.punit->GetBaseStatus().profession!=Unit::Profession::e_healer){
				//�s���ł���͈͂����߂�
				std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);
				for(const LogElement::UnitLogData &logData:*pUnitLogDataList){
					//���j�b�g�ɂ��i�q�_�N���s����ǉ��A���������삵�Ă������j�b�g�͏���
					if(logData.punit!=attackerData.punit && logData.punit!=operatedUnit.punit){
						latticeField->BecomeImpassibleLattice(logData.punit,attackerData.punit->GetBattleStatus().team);
					}
				}
				//���샆�j�b�g�����Ȃ����̌�q���j�b�g�̔�e���X�g�쐬
				const std::vector<bool> notExistJudge=JudgeAttackableList(latticeField,attackerData,friendBackUnit);
				//�s���͈͂ɑ��샆�j�b�g�̉e����^����
				Unit virtualUnit=*operatedUnit.punit;
				virtualUnit.Warp(operatedUnit.pos);
				latticeField->BecomeImpassibleLattice(&virtualUnit,attackerData.punit->GetBattleStatus().team);
				//���샆�j�b�g�����鎞�̌�q���j�b�g�̔�e���X�g�쐬
				const std::vector<bool> existJudge=JudgeAttackableList(latticeField,attackerData,friendBackUnit);
				//judgeMatrix�ւ̊i�[
				std::vector<std::pair<bool,bool>> attackerJudgeList(friendBackUnitCount,std::make_pair(false,false));
				for(size_t i=0;i<friendBackUnitCount;i++){
					attackerJudgeList[i]=std::make_pair(notExistJudge[i],existJudge[i]);
				}
				judgeMatrix.push_back(attackerJudgeList);
			}
		}
		//�����v�Z
		int notExistCount=0,existCount=0;//���������Ȃ����E���鎞���ꂼ��̍U�������\���̂����q���j�b�g�̐�
		for(size_t j=0;j<friendBackUnitCount;j++){
			//������q���j�b�g[j]�ɑ΂��čU���\�ȓG���j�b�g�����݂��邩�ǂ������v�Z
			bool notExistFlag=false,existFlag=false;
			for(size_t i=0,attackerCount=judgeMatrix.size();i<attackerCount;i++){
				notExistFlag=notExistFlag | judgeMatrix[i][j].first;
				existFlag=existFlag | judgeMatrix[i][j].second;
			}
			//���Z
			if(notExistFlag){
				notExistCount++;
			}
			if(existFlag){
				existCount++;
			}
		}
		//a,b�̊����ŕ]��
		if(notExistCount==0){
			//���샆�j�b�g�����Ȃ��Ă��N���U������Ȃ���
			evaluate=-1;
		} else if(existCount==notExistCount){
			//���샆�j�b�g�����Ă��U���ۊ֌W���ω����Ȃ���
			evaluate=0;
		} else if(existCount>0){
			//���샆�j�b�g���ꕔ�̍U����h���ł��鎞
			evaluate=1;
		} else if(existCount==0){
			//���샆�j�b�g���S�Ă̍U����h���ł��鎞
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
		//����
		rubricStr="Bad";
		edgeColor=GetColor(96,96,196);
		break;
	case(1):
		//����
		rubricStr="Not good";
		edgeColor=GetColor(128,128,196);
		break;
	case(2):
		//����
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
		//���m��d���������Ȃ�
		comment="���m��d�������g���Ă݂悤�I";
		break;
	case(0):
		//Bad�]��������
		comment="��q���j�b�g�ƓG���j�b�g�̊ԂɑO�q���j�b�g��z�u���邱�ƂŌ�q���j�b�g������Ă݂悤�I";
		break;
	case(1):
		//Not Good�]��������
		comment="��������r�I�����ꏊ�ɑO�q���j�b�g��z�u���āA��q���j�b�g�������������Ă����悤�I";
		break;
	case(2):
		//����ȏ�̕]��
		comment="�������ƂȂ��ł��I�O�q���j�b�g�Ō�q���j�b�g������������Ă��܂��I";
		break;
	}
	return comment;
}

std::string ProtectFriend::GetWholeLookBackActionEmpty()const{
	return "��q���j�b�g���O�q���j�b�g���痣�ꂷ���āA�K�؂ȃT�|�[�g�����ɂ����Ȃ��Ă��܂��񂩁H";
}

std::string ProtectFriend::GetSubmissionExplanation()const{
	return "�ˎ�E�����m�E�q�����Ƃ�������q���j�b�g�͑ł���ア�̂ŁA\n���m�E�d�����Ƃ������O�q���j�b�g��ǂɂ��Ď���Ă����悤�I";
}

bool ProtectFriend::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& (battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_armer
			|| battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_soldier));
}

