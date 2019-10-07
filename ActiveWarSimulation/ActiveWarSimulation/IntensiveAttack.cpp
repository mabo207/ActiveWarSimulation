#include"DxLib.h"
#include"IntensiveAttack.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include<algorithm>

int IntensiveAttack::RubricEvaluate(const BattleSceneData * const battleData)const{
	//- ��O����
	//	- �U�����Ȃ�(-1)
	//- �]��
	//	1. �U��������HP�����̒Ⴂ���j�b�g��3�̈ȏア��
	//	2. �U��������HP�����̒Ⴂ���j�b�g��2�̂���
	//	3. �U��������HP�����̒Ⴂ���j�b�g��1�̂���
	//	4. �U��������HP�����̒Ⴂ���j�b�g�����Ȃ�
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(battleData->m_scoreObserver->GetLatestLog());
	int evaluate;
	const int maxEvaluate=3;
	if(!attackLog){
		//���O��AttackLog�łȂ��ꍇ�́u�U�������Ȃ������v�Ɣ��f�ł���
		evaluate=-1;
	} else{
		//���j�ł��邩�̔���
		const LogElement::UnitLogData aimedData=attackLog->GetAimedUnitData();
		if(aimedData.hp<=attackLog->GetAttackInfo().damage){
			//���j���Ă����ꍇ�́A�c��HP�����̏����Ɋւ�炸�ő�]����^����
			evaluate=maxEvaluate;
		} else{
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
			//�U�����肪���Ԗڂ�����������A���������ɒ���
			size_t order=0;//���ʁi�������HP�����̏������L���������̂��邩�j���L�^�A�������l������
			for(size_t i=0,vecSize=hpRateVec.size();i<vecSize;i++){
				if(hpRateVec[i].first==aimedData.punit){
					//�U�����肪��������
					break;
				} else{
					//�U�����肪������Ȃ������ꍇ�A���ʍX�V����
					if(i+1<vecSize && hpRateVec[i].second!=hpRateVec[i+1].second){
						//[i]��HP������[i+1]��HP�����ɓ������Ȃ��Ȃ�A[0]~[i]�̗v�f�͍U�������菇�ʂ���
						order=i+1;//���ʍX�V([0]���琔����̂�+1����)
					}
				}
			}
			//���ԂɊ�Â��]��
			if(order==0){
				//�U��������HP�����̍����L���������Ȃ�
				evaluate=maxEvaluate;
			} else if(order==1){
				//�U��������HP�����̍����L������1�̂���
				evaluate=2;
			} else if(order==2){
				//2�̂���
				evaluate=1;
			} else{
				//3�̈ȏ�
				evaluate=0;
			}
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
		//����
		rubricStr="Bad";
		edgeColor=GetColor(96,96,196);
		break;
	case(1):
		//�܂��܂�
		rubricStr="OK";
		edgeColor=GetColor(128,128,196);
		break;
	case(2):
		//�ǂ�
		rubricStr="Good!!";
		edgeColor=GetColor(196,196,64);
		break;
	case(3):
		//�ƂĂ��ǂ�
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
		//�U�����Ă��Ȃ�
		comment="�܂��͐ϋɓI�ɍU�����Ă݂悤�I";
		break;
	case(0):
		//HP�������������j�b�g��_���Ă���(3�l�ȏ�)
		comment="�o���o���ɓG���U������̂ł͂Ȃ��A�P�̂̓G���W���I�ɍU�������������R����邽�߂ɂ����ʓI�ł���I";
		break;
	case(1):
		//HP�������������j�b�g��_���Ă���(2�l�ȏ�)
		comment="�W���U�������₷�����邽�߂ɁA�U���˒��̒Z���L�����͑O�A�˒��̒����L�����͌��ɔz�u���邱�Ƃ�S�����Ă݂悤�I";
		break;
	case(2):
		//HP�������������j�b�g��_���Ă���(1�l�ȏ�)
		comment="�W���U�����悭�S�����Ă��܂����A����Ă���G�������Ƃ��Ă��邩������܂���B";
		break;
	case(3):
		//�W���U���ł��Ă���
		comment="�������ƂȂ��ł��I�W���U���œG�̐�͂���鎖���ł��Ă��܂��I";
		break;
	}
	return comment;
}

std::string IntensiveAttack::GetWholeLookBackActionEmpty()const{
	return "�ϋɓI�ɍU�����Ă݂悤�I";
}

std::string IntensiveAttack::GetSubmissionExplanation()const{
	return "�U���͍ő�̖h��I\n�����G���W���U�����đf�����G��͂�����Ă݂悤�I";
}

bool IntensiveAttack::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession!=Unit::Profession::e_healer);
}
