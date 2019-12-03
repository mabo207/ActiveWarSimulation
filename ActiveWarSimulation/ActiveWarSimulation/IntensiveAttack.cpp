#include"DxLib.h"
#include"IntensiveAttack.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include<algorithm>

SubmissionEvaluation IntensiveAttack::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- ��O����
	//	- �U�����Ȃ�(-1)
	//	- ���j���͕K���ő�]��(4)
	//	- �c��1�̂̎��͌��j���ȊO�͕]�����Ȃ��i-1�j
	//	- �c��2�̂̎��͍ō��]���łȂ����ɕ]��2�Ƃ���B(2)
	//- �]��
	//	1. �U��������HP�����̒Ⴂ���j�b�g��3�̈ȏア��
	//	2. �U��������HP�����̒Ⴂ���j�b�g��2�̂���
	//	3. �U��������HP�����̒Ⴂ���j�b�g��1�̂���
	//	4. �U��������HP�����̒Ⴂ���j�b�g�����Ȃ�
	//���O�_�Ƃ��āA�u���������̖��Ƃ��āAexcellent�]�������j���Ə���U�����ɏo�Ă��܂����牺��ł��ō��]�����������o���Ă��܂��̂ł́v��������B
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	SubmissionEvaluation evaluate;
	const SubmissionEvaluation maxEvaluate=SubmissionEvaluation::e_excellent;
	if(!attackLog){
		//���O��AttackLog�łȂ��ꍇ�́u�U�������Ȃ������v�Ɣ��f�ł���
		evaluate=SubmissionEvaluation::e_noevaluation;
	} else{
		//���j�ł��邩�̔���
		const LogElement::UnitLogData aimedData=attackLog->GetAimedUnitData();
		//�S�Ă̓G��HP�������o��
		std::vector<std::pair<const Unit *,int>> hpRateVec;
		for(const LogElement::UnitLogData &unitData:attackLog->m_unitDataList){
			if(unitData.punit->GetBattleStatus().team==Unit::Team::e_enemy){
				//�S�Ă̓G�ɂ��Ċi�[
				hpRateVec.push_back(std::make_pair(unitData.punit,32768*unitData.hp/unitData.punit->GetBaseStatus().maxHP));//int�Ōv�Z����̂Ő؂�̂Ă��|���A������x�傫�����l�������邱�ƂŊ����̃\�[�g���\�ɂȂ�B
			}
		}
		//HP�����̒Ⴂ���Ƀ\�[�g����
		const auto sortFunc=[](const std::pair<const Unit *,int> left,const std::pair<const Unit *,int> right){
			if(left.second<right.second){
				return true;
			} else if(left.second>right.second){
				return false;
			}
			return left.first<right.first;
		};
		std::sort(hpRateVec.begin(),hpRateVec.end(),sortFunc);
		//�G�̐����L�^(�]���ɗp����)
		const size_t enemyNum=hpRateVec.size();
		//�U�����肪���Ԗڂ�����������A���������ɒ���
		size_t order=0;//���ʁi�������HP�����̏������L���������̂��邩�j���L�^�A�������l������
		for(size_t i=0;i<enemyNum;i++){
			if(hpRateVec[i].first==aimedData.punit){
				//�U�����肪��������
				break;
			} else{
				//�U�����肪������Ȃ������ꍇ�A���ʍX�V����
				if(i+1<enemyNum && hpRateVec[i].second!=hpRateVec[i+1].second){
					//[i]��HP������[i+1]��HP�����ɓ������Ȃ��Ȃ�A[0]~[i]�̗v�f�͍U�������菇�ʂ���
					order=i+1;//���ʍX�V([0]���琔����̂�+1����)
				}
			}
		}
		//���ԂƓG���Ɋ�Â��]��
		if(aimedData.hp<=attackLog->GetAttackInfo().damage){
			//���j���Ă����ꍇ�́A�c��HP�����̏����Ɋւ�炸�ő�]����^����
			evaluate=maxEvaluate;
		} else if(enemyNum==1){
			//�c��1�̂̏ꍇ�́A���j���ȊO�]���Ȃ�
			evaluate=SubmissionEvaluation::e_noevaluation;
		} else if(enemyNum==2 && order>0){
			//�c��2�̂̏ꍇ�́A�ő�]���łȂ����̕]����ʏ�ƕς���
			evaluate=SubmissionEvaluation::e_ok;
		} else if(order==0){
			//�U��������HP�����̍����L���������Ȃ�
			evaluate=maxEvaluate;
		} else if(order==1){
			//�U��������HP�����̍����L������1�̂���
			evaluate=SubmissionEvaluation::e_good;
		} else if(order==2){
			//2�̂���
			evaluate=SubmissionEvaluation::e_ok;
		} else{
			//3�̈ȏ�
			evaluate=SubmissionEvaluation::e_bad;
		}
	}

	return evaluate;
}

std::string IntensiveAttack::GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const{
	std::string comment;
	if(mostFrequentEvaluate==SubmissionEvaluation::e_noevaluation){
		//�U�����Ă��Ȃ�
		comment="�܂��͐ϋɓI�ɍU�����Ă݂悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_bad){
		//HP�������������j�b�g��_���Ă���(3�l�ȏ�)
		comment="�o���o���ɓG���U������̂ł͂Ȃ��A�P�̂̓G���W���I�ɍU�������������R����邽�߂ɂ����ʓI�ł���I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_ok){
		//HP�������������j�b�g��_���Ă���(2�l�ȏ�)
		comment="�W���U�������₷�����邽�߂ɁA�U���˒��̒Z���L�����͑O�A�˒��̒����L�����͌��ɔz�u���邱�Ƃ�S�����Ă݂悤�I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_good){
		//HP�������������j�b�g��_���Ă���(1�l�ȏ�)
		comment="�W���U�����悭�S�����Ă��܂����A����Ă���G�������Ƃ��Ă��邩������܂���B";
	}else if(mostFrequentEvaluate==SubmissionEvaluation::e_excellent){
		//�W���U���ł��Ă���
		comment="�������ƂȂ��ł��I�W���U���œG�̐�͂���鎖���ł��Ă��܂��I";
	}
	return comment;
}

std::string IntensiveAttack::GetWholeLookBackActionEmpty()const{
	return "�ϋɓI�ɍU�����Ă݂悤�I";
}

std::string IntensiveAttack::GetSubmissionExplanation()const{
	return "�U���͍ő�̖h��I\n�����G���W���U�����f�����G��͂���낤�I";
}

bool IntensiveAttack::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession!=Unit::Profession::e_healer);
}

std::string IntensiveAttack::GetReason(SubmissionEvaluation rubric)const{
	if(rubric==SubmissionEvaluation::e_noevaluation){
		//�`����s��Ȃ�
		return "";
	} else if(rubric==SubmissionEvaluation::e_bad){
		return "�c��HP�����Ȃ��G���قƂ�ǂ��A����̓G���W���U�����������ǂ��񂶂�Ȃ��H";
	} else if(rubric==SubmissionEvaluation::e_ok){
		return "�ʁX�̓G��_���Ă��Ȃ����H�W���U�����厖���B";
	} else if(rubric==SubmissionEvaluation::e_good){
		return "�����Ǝ荠�ɓ|����G�����邩������Ȃ��A�T���Ă݂悤�B";
	} else if(rubric==SubmissionEvaluation::e_excellent){
		return "�c��HP�����Ȃ��G���W�����čU���ł��Ă��܂��I";
	}
	return "";
}
