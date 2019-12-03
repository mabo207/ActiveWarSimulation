#include"DxLib.h"
#include"ProtectFriend.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"WaitLog.h"

SubmissionEvaluation ProtectFriend::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- �]���ɗp����f�[�^
	//	- �����ꂩ�̓G���j�b�g�̎��̍s���ōU���\�ƂȂ閡����q���j�b�g�̐�(a)
	//	- ���샆�j�b�g���}�b�v������������̏�L�̃f�[�^(b)
	//- ��O����
	//	- b==0
	//- �]��
	//	1. (b-a)/b==0(�N�����Ă��Ȃ�)
	//	2. (b-a)/b<1(�N�����Ă���)
	//	3. (b-a)/b==1(�S�Ă̖��������Ă���)
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(evaluateLog);
	const std::vector<LogElement::UnitLogData> *pUnitLogDataList=nullptr;
	std::function<LogElement::UnitLogData()> getOperatedUnitData;
	if(attackLog){
		pUnitLogDataList=&attackLog->m_unitDataList;
		getOperatedUnitData=[attackLog](){return attackLog->GetOperateUnitData();};
	} else if(waitLog){
		pUnitLogDataList=&waitLog->m_unitDataList;
		getOperatedUnitData=[waitLog](){return waitLog->GetOperateUnitData();};
	}
	SubmissionEvaluation evaluate;
	if(pUnitLogDataList==nullptr){
		//�����ɗ��邱�Ƃ͂Ȃ��͂������A�ꉞ�G���[����
		evaluate=SubmissionEvaluation::e_noevaluation;
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
				std::shared_ptr<LatticeBattleField> latticeField=LatticeBattleField::Create(field,stageSize,operatedUnit.punit,false);
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
			evaluate=SubmissionEvaluation::e_noevaluation;
		} else if(existCount==notExistCount){
			//���샆�j�b�g�����Ă��U���ۊ֌W���ω����Ȃ���
			evaluate=SubmissionEvaluation::e_bad;
		} else if(existCount>0){
			//���샆�j�b�g���ꕔ�̍U����h���ł��鎞
			evaluate=SubmissionEvaluation::e_good;
		} else if(existCount==0){
			//���샆�j�b�g���S�Ă̍U����h���ł��鎞
			evaluate=SubmissionEvaluation::e_excellent;
		}
	}

	return evaluate;
}

std::string ProtectFriend::GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const{
	std::string comment;
	if(mostFrequentEvaluate==SubmissionEvaluation::e_noevaluation){
		//���m��d���������Ȃ�
		comment="���m��d�������g���Ă݂悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_bad){
		//Bad�]��������
		comment="��q���j�b�g�ƓG���j�b�g�̊ԂɑO�q���j�b�g��z�u���邱�ƂŌ�q���j�b�g������Ă݂悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_good){
		//Not Good�]��������
		comment="��������r�I�����ꏊ�ɑO�q���j�b�g��z�u���āA��q���j�b�g�������������Ă����悤�I";
	}else if(mostFrequentEvaluate==SubmissionEvaluation::e_excellent){
		//����ȏ�̕]��
		comment="�������ƂȂ��ł��I�O�q���j�b�g�Ō�q���j�b�g������������Ă��܂��I";
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

std::string ProtectFriend::GetReason(SubmissionEvaluation rubric)const{
	if(rubric==SubmissionEvaluation::e_noevaluation){
		//�`����s��Ȃ�
		return "";
	} else if(rubric==SubmissionEvaluation::e_bad){
		return "��q���j�b�g�ւ̍U�����ǂ���h�����Ƃ��ł��Ȃ��������B";
	} else if(rubric==SubmissionEvaluation::e_good){
		return "��q���j�b�g�ւ̍U�����ꕔ�h���Ă����I";
	} else if(rubric==SubmissionEvaluation::e_excellent){
		return "��q���j�b�g�ւ̍U�������ׂăV���b�g�A�E�g�����悤���I";
	}
	return "";
}
