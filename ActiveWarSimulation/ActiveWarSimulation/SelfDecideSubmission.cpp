#include"DxLib.h"
#include"SelfDecideSubmission.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
#include"ToolsLib.h"

namespace{
	const int width=500;
	const int height=200;
}

//--------------SelfDecideSubmission---------------
bool SelfDecideSubmission::JudgeEvaluatedOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_archer);
}

void SelfDecideSubmission::RubricEvaluate(const BattleSceneData * const battleData){
	//- ��O����
	//	- �U�����Ȃ�(-1)
	//- �]��
	//	0. �����������A�G���j�b�g�U���˒��ȉ�(�G�̎˒��������̎˒���蒷���ꍇ�͖������ŒB��������̂Ƃ���)
	//	1. ���[�g�������A�����j�b�g�U���˒��ȉ�
	//	2. ���[�g�������A�G�̈ړ������ȉ�
	//	3. 2�ȏ�̕]��
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(battleData->m_scoreObserver->GetLatestLog());
	int evaluate;
	if(!attackLog){
		//���O��AttackLog�łȂ��ꍇ�́u�U�������Ȃ������v�Ɣ��f�ł���
		evaluate=-1;
	} else{
		//�����Ɋւ���]��������
		//�������������߂�
		const float directDistance=(attackLog->GetOperateUnitData().pos-attackLog->GetAimedUnitData().pos).size();
		//��e���j�b�g���s�����j�b�g�ւ̃��[�g���������߂�
		std::shared_ptr<LatticeBattleField> latticeField=battleData->CalculateLatticeBattleField(false);
		const Unit *operatedUnit=attackLog->GetOperateUnitData().punit;
		const Unit *aimedUnit=attackLog->GetAimedUnit();
		for(const LogElement::UnitLogData &logData:attackLog->m_unitDataList){
			//���j�b�g�̊i�q�_�ǉ�(��e���j�b�g����������z�肵�Ă���)
			if(logData.punit!=operatedUnit && logData.punit!=aimedUnit){
				latticeField->BecomeImpassibleLattice(logData.punit,aimedUnit->GetBattleStatus().team);
			}
		}
		std::vector<float> distVec=latticeField->CalculateRouteDistance(aimedUnit->getPos(),{operatedUnit->getPos()});
		const float routeDistance=distVec.front();
		//�]��(���������画�肵�Ă���)
		if(routeDistance>=attackLog->GetAimedUnit()->GetMaxMoveDistance()){
			evaluate=3;
		} else if(routeDistance>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength()){
			evaluate=2;
		} else if(directDistance>=attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()
			|| attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength())
		{
			evaluate=1;
		} else{
			evaluate=0;
		}
	}
	//�]���̒~�ς����ď������I��
	m_rubricList.push_back(evaluate);
}

void SelfDecideSubmission::WholeLookBack(){
	if(m_rubricList.empty()){
		//1����ˎ�ōU�����Ă��Ȃ���
		m_wholeComment="�����Ǝˎ�ōU�����Ă݂悤�I";
		return;
	}
	//�ŕp�l�����߂�
	std::map<int,size_t> frequencyMap;
	for(const int &rubric:m_rubricList){
		std::map<int,size_t>::iterator it=frequencyMap.find(rubric);
		if(it==frequencyMap.end()){
			//�܂�rubric��1��������Ă��Ȃ��ꍇ�́AfrequencyMap�ɒǉ�
			frequencyMap.insert(std::make_pair(rubric,1));
		} else{
			//���Ɍ������Ă���ꍇ�́A�񐔂�1���₷
			it->second++;
		}
	}
	std::pair<int,size_t> mostFrequent=*frequencyMap.begin();
	for(const std::pair<int,size_t> &pair:frequencyMap){
		if(mostFrequent.first==-1){
			//mostFrequent���u�U�����Ă��Ȃ��v�ł���ꍇ�́A�K���㏑������
			mostFrequent=pair;
		} else if(pair.second>mostFrequent.second && pair.first!=-1){
			//�u�U�����Ă��Ȃ��v�ȊO�̕p�x���傫�����̂ɍX�V
			mostFrequent=pair;
		}
	}
	//�ŕp�l�̃��[�u���b�N�ɉ����ăR�����g���i�[
	switch(mostFrequent.first){
	case(-1):
		//�U�����Ă��Ȃ�
		m_wholeComment="�����Ǝˎ�ōU�����Ă݂悤�I";
		break;
	case(0):
		//�G���j�b�g�̍U���˒�������̍U��
		m_wholeComment="�G�̎˒��O����U�����鎖���ӎ����Ă݂�Ɨǂ���I";
		break;
	case(1):
		//���[�g�������U���˒��ȉ�
		m_wholeComment="�|�ōU�����鎞�́A��Q���□���z���ɍU�����Ă݂悤�I";
		break;
	case(2):
		//���[�g�������G�̈ړ������ȉ�
		m_wholeComment="�G�����̍s���łǂ��܂ōs���邩���l���āA�G�̍U�����󂯂Ȃ��悤�Ȉʒu�ōU�����邱�Ƃ��ӎ����Ă݂悤�I";
		break;
	case(3):
		//����ȏ�̕]��
		m_wholeComment="�������ƂȂ��ł��I��Q���□�������܂��g���āA���S�ȏꏊ����U���ł��Ă��܂��I";
		break;
	}
}

void SelfDecideSubmission::DrawSubmission(int x,int y,int font)const{
	const std::string submissionStr="�n�`�□���L�����̕ǂ𗘗p���āA\n���S�n�т���U�����Ă݂悤�I";
	DrawBox(x,y,x+width,y+height,GetColor(64,192,64),TRUE);
	DrawBox(x,y,x+width,y+height,GetColor(192,255,192),FALSE);
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10,submissionStr.c_str(),GetColor(255,255,255),font,2);
}

void SelfDecideSubmission::DrawRubric(int x,int y,int font)const{
	if(!m_rubricList.empty()){
		//���[�u���b�N�]���̕������`
		std::string rubricStr;
		unsigned int edgeColor;
		switch(m_rubricList.back()){
		case(-1):
			rubricStr="";
			edgeColor=GetColor(0,0,0);
			break;
		case(0):
			//����
			rubricStr="Worst";
			edgeColor=GetColor(128,0,196);
			break;
		case(1):
			//����
			rubricStr="Bad";
			edgeColor=GetColor(96,96,196);
			break;
		case(2):
			//����
			rubricStr="Not good";
			edgeColor=GetColor(128,128,196);
			break;
		case(3):
			//����
			rubricStr="Good!!";
			edgeColor=GetColor(196,196,64);
			break;
		}
		//�`��
		DrawStringToHandle(x,y,rubricStr.c_str(),GetColor(255,255,255),font,edgeColor);
	}
}

void SelfDecideSubmission::DrawWholeLookBack(int x,int y,int font)const{
	DrawBox(x,y,x+width,y+height,GetColor(64,192,64),TRUE);
	DrawBox(x,y,x+width,y+height,GetColor(192,255,192),FALSE);
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10,m_wholeComment.c_str(),GetColor(255,255,255),font,2);
}
