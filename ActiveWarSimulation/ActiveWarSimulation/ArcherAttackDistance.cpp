#include"DxLib.h"
#include"ArcherAttackDistance.h"
#include"BattleSceneData.h"
#include"AttackLog.h"
//���z�}�`�\�z�̂��߂Ɏg��
#include"Edge.h"

//----------------ArcherAttackDistance----------------
const std::array<SubmissionEvaluation,4> ArcherAttackDistance::s_evaluate={
	SubmissionEvaluation::e_bad
	,SubmissionEvaluation::e_ok
	,SubmissionEvaluation::e_good
	,SubmissionEvaluation::e_excellent
};

std::shared_ptr<SubmissionRuleBase::InAdvanceCalculateDataBase> ArcherAttackDistance::CalculateInAdvanceData(
	const std::vector<BattleObject *> &field
	,const Vector2D stageSize
	,const std::shared_ptr<const LogElement> &evaluateLog)const
{
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	if(!attackLog){
		//���O��AttackLog�łȂ��ꍇ�͈Ӗ��̂Ȃ��f�[�^��Ԃ�
		return std::shared_ptr<InAdvanceCalculateDataBase>();
	} else{
		//��e���j�b�g���s�����j�b�g�ւ̃��[�g�ɂ��Ă̎��O�f�[�^��Ԃ�
		const LogElement::UnitLogData operatedUnit=attackLog->GetOperateUnitData();
		const LogElement::UnitLogData aimedUnit=attackLog->GetAimedUnitData();
		return CalculateInAdvanceRouteData(field,stageSize,attackLog->m_unitDataList,operatedUnit,aimedUnit);
	}
}

SubmissionEvaluation ArcherAttackDistance::InAdvanceDataEvaluate(
	const std::shared_ptr<InAdvanceCalculateDataBase> &inAdvanceData
	,const std::vector<BattleObject *> &field
	,const Vector2D stageSize
	,const std::shared_ptr<const LogElement> &evaluateLog)const
{
	//- ��O����
	//	- �U�����Ȃ�(-1)
	//- �]��
	//	0. �����������A�G���j�b�g�U���˒��ȉ�(�G�̎˒��������̎˒���蒷���ꍇ�͖������ŒB��������̂Ƃ���)
	//	1. ���[�g�������A�����j�b�g�U���˒��ȉ�
	//	2. ���[�g�������A�G�̈ړ������ȉ�
	//	3. 2�ȏ�̕]��
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	const std::shared_ptr<InAdvanceRouteData> routeData=std::dynamic_pointer_cast<InAdvanceRouteData>(inAdvanceData);
	SubmissionEvaluation evaluate;
	if(!attackLog || !routeData){
		//���O��AttackLog�łȂ��ꍇ�́u�U�������Ȃ������v�Ɣ��f�ł���
		evaluate=SubmissionEvaluation::e_noevaluation;
	} else{
		//�����Ɋւ���]��������
		FpsMeasuring fps;
		fps.RecordTime();
		//�������������߂�
		const float directDistance=(attackLog->GetOperateUnitData().pos-attackLog->GetAimedUnitData().pos).size();
		//���O�f�[�^��p���Ĕ�e���j�b�g���s�����j�b�g�ւ̃��[�g���������߂�
		const LogElement::UnitLogData operateUnit=attackLog->GetOperateUnitData();
		const float routeDistance=CalculateRouteDistance(routeData,operateUnit);
		//2���j�b�g�Ԃɏ�Q�������邩�̔���
		const Vector2D p0=attackLog->GetOperateUnitData().pos,p1=attackLog->GetAimedUnitData().pos;//2���j�b�g�̈ʒu		
		const Edge edge(p0,p1-p0,Shape::Fix::e_static);//2���j�b�g�̒��a�ɂ���č\������钷���`���\�z
		//�S�Ă̐}�`�ɑ΂��ďd�Ȃ��Ă��邩�𔻒�
		bool existSomethingInSpace=false;
		for(const BattleObject *obj:field){
			if(obj->GetFix()!=Shape::Fix::e_ignore
				&& (obj!=attackLog->GetAimedUnit() && obj!=attackLog->GetOperateUnitData().punit)
				&& (edge.JudgeCross(obj->GetHitJudgeShape()) || edge.JudgeInShape(obj->GetHitJudgeShape())))
			{
				std::shared_ptr<Shape> operatedShape=attackLog->GetOperateUnitData().punit->GetUnitCircleShape()->VCopy();
				operatedShape->Move(attackLog->GetOperateUnitData().pos-operatedShape->GetPosition());
				if(!obj->GetHitJudgeShape()->JudgeInShape(operatedShape.get())){
					//�u��Q��������v�̏���
					//- ���݂��Ȃ��n�`�ł͂Ȃ�
					//- 2���j�b�g�Ƃ͈قȂ���̂ł���
					//- edge������������edge�ƌ������Ă���
					//- operatedUnit��obj�ɓ����Ă��Ȃ�
					existSomethingInSpace=true;
					break;
				}
			}
		}
		//�]��(���������画�肵�Ă���)
		if(routeDistance>=attackLog->GetAimedUnit()->GetMaxMoveDistance()+attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength() || routeDistance<0.0f){
			//routeDistance<0.0f�̎��́A���B�o�H�����݂��Ȃ��Ƃ������ƂȂ̂ŁA���[�g�������G�̈ړ�������蒷���̂Ɠ��������ɂȂ�B
			evaluate=s_evaluate[3];
		} else if(existSomethingInSpace && routeDistance>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength()){
			evaluate=s_evaluate[2];
		} else if(directDistance>=attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()
			|| attackLog->GetAimedUnit()->GetBattleStatus().weapon->GetLength()>=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength())
		{
			evaluate=s_evaluate[1];
		} else{
			evaluate=s_evaluate[0];
		}
	}

	return evaluate;
}

std::string ArcherAttackDistance::GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const{
	std::string comment;
	if(mostFrequentEvaluate==SubmissionEvaluation::e_noevaluation){
		//�U�����Ă��Ȃ�
		comment="�����Ǝˎ�ōU�����Ă݂悤�I";
	} else if(mostFrequentEvaluate==s_evaluate[0]){
		//�G���j�b�g�̍U���˒�������̍U��
		comment="�G�̎˒��O����U�����鎖���ӎ����Ă݂�Ɨǂ���I";
	} else if(mostFrequentEvaluate==s_evaluate[1]){
		//���[�g�������U���˒��ȉ�
		comment="�|�ōU�����鎞�́A��Q���□���z���ɍU�����Ă݂悤�I";
	} else if(mostFrequentEvaluate==s_evaluate[2]){
		//���[�g�������G�̈ړ������ȉ�
		comment="�G�����̍s���łǂ��܂ōs���邩���l���āA�G�̍U�����󂯂Ȃ��悤�Ȉʒu�ōU�����邱�Ƃ��ӎ����Ă݂悤�I";
	} else if(mostFrequentEvaluate==s_evaluate[3]){
		//����ȏ�̕]��
		comment="�������ƂȂ��ł��I��Q���□�������܂��g���āA���S�ȏꏊ����U���ł��Ă��܂��I";
	}
	return comment;
}

std::string ArcherAttackDistance::GetWholeLookBackActionEmpty()const{
	return "�����Ǝˎ�ōU�����Ă݂悤�I";
}

std::string ArcherAttackDistance::GetSubmissionExplanation()const{
	return "�˒��̒����ˎ�́A\n�n�`�□���L�����̕ǂ𗘗p���Ĉ��S�n�т���U�����Ă݂悤�I";
}

bool ArcherAttackDistance::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_archer);
}

std::string ArcherAttackDistance::GetReason(SubmissionEvaluation rubric)const{
	if(rubric==SubmissionEvaluation::e_noevaluation){
		//�`����s��Ȃ�
		return "";
	} else if(rubric==s_evaluate[0]){
		return "�G�����̏�ōU���ł��邭�炢�ɋ߂��ōU����������Ă��I";
	} else if(rubric==s_evaluate[1]){
		return "��Q��������ɂȂ�����U�������G�̔����ɍ����₷��������Ȃ��H";
	} else if(rubric==s_evaluate[2]){
		return "��Q���z���ɍU���ł��Ă邯�ǁA�ĊO�G�͉�荞��ōU���ł������B";
	} else if(rubric==s_evaluate[3]){
		return "���S�n�т���̍U���A�Ƃ��Ă��ǂ������I�I";
	}
	return "";
}
