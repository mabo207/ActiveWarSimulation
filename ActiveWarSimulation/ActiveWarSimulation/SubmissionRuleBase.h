#ifndef DEF_SUBMISSIONRULEBASE_H
#define DEF_SUBMISSIONRULEBASE_H

#include<string>
#include<memory>
#include<vector>

#include"LogElement.h"
#include"LatticeBattleField.h"
#include"SubmissionEvaluation.h"

struct BattleSceneData;
class Unit;

//�T�u�~�b�V�����]�����̃��[����\�����邽�߂̊��N���X
class SubmissionRuleBase{
public:
	struct InAdvanceCalculateDataBase{
		//���O�v�Z�������ʂ��i�[����N���X�A�p�����ėp����
		virtual ~InAdvanceCalculateDataBase()=default;
	};
	struct InAdvanceRouteData:public InAdvanceCalculateDataBase{
		//���[�g�v�Z�ɂ��āA���O�v�Z�ł�����e
		InAdvanceRouteData(const std::shared_ptr<LatticeBattleField> &latticeField
			,const std::vector<LatticeBattleField::LatticeDistanceInfo> &distanceInfoVec)
			:InAdvanceCalculateDataBase()
			,m_latticeField(latticeField)
			,m_distanceInfoVec(distanceInfoVec)
		{}
		std::shared_ptr<LatticeBattleField> m_latticeField;//�i�q�_�f�[�^
		std::vector<LatticeBattleField::LatticeDistanceInfo> m_distanceInfoVec;//�����}�b�v
	};

	virtual ~SubmissionRuleBase()=default;
	//BattleSceneData��p�������[�u���b�N�]��
	SubmissionEvaluation RubricEvaluate(const BattleSceneData * const battleData)const;
	//����̃��O��p�������[�u���b�N�]��
	SubmissionEvaluation RubricEvaluate(
		const std::vector<BattleObject *> &field
		,const Vector2D stageSize
		,const std::shared_ptr<const LogElement> &evaluateLog)const
	{
		//���O�v�Z�f�[�^���v�Z���āA������g���Čv�Z����֐�
		const auto inAdvanceData=CalculateInAdvanceData(field,stageSize,evaluateLog);
		return InAdvanceDataEvaluate(inAdvanceData,field,stageSize,evaluateLog);
	}
	//���[�u���b�N�]���̂��߂̎��O�f�[�^���v�Z����i�f�t�H���g�ł͉������Ȃ��j
	virtual std::shared_ptr<InAdvanceCalculateDataBase> CalculateInAdvanceData(
		const std::vector<BattleObject *> &field
		,const Vector2D stageSize
		,const std::shared_ptr<const LogElement> &evaluateLog)const
	{
		//�f�t�H���g�ł́A�Ӗ��̂Ȃ��f�[�^��Ԃ�
		return std::shared_ptr<InAdvanceCalculateDataBase>();
	}
	//���O�f�[�^��p���ĕ]������BCalculateInAdvanceData()���f�t�H���g�̂܂܂Ȃ�A�]�������̑S�Ă������ɏ������B
	virtual SubmissionEvaluation InAdvanceDataEvaluate(
		const std::shared_ptr<InAdvanceCalculateDataBase> &inAdvanceData
		,const std::vector<BattleObject *> &field
		,const Vector2D stageSize
		,const std::shared_ptr<const LogElement> &evaluateLog)const=0;
	virtual std::string GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const=0;//�ł������������[�u���b�N�]���ɂ��āA�����I�ɂȃR�����g��Ԃ�
	virtual std::string GetWholeLookBackActionEmpty()const=0;//���[���ɊY�������s���������Ȃ��������̑����I�]���̕������Ԃ�
	virtual std::string GetSubmissionExplanation()const=0;//�T�u�~�b�V�����̐�������Ԃ�
	virtual bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const=0;//�]�����s�����ǂ���
	virtual std::string GetReason(SubmissionEvaluation rubric)const=0;//���[�u���b�N�]���̗��R�̕��͂�Ԃ�

protected:
	SubmissionRuleBase()=default;
	//�悭�]���ɗp����������v�Z����֐��AUnit *�łȂ�UnitLogData��p���Čv�Z�����Ȃ��Ƃ����Ȃ�
	float CalculateRouteDistance(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;//operatedUnit����aimedUnit�܂ł̌o�H�������v�Z����
	float CalculateRouteDistance(const std::shared_ptr<InAdvanceRouteData> &inAdvanceData,const LogElement::UnitLogData operatedUnit)const;//operatedUnit����aimedUnit�܂ł̌o�H�������v�Z����(���O�f�[�^�g�p)
	float CalculateRouteDistance(const std::shared_ptr<InAdvanceRouteData> &inAdvanceData,const Vector2D from)const;//from����aimedUnit�܂ł̌o�H�������v�Z����(���O�f�[�^�g�p)
	std::shared_ptr<InAdvanceRouteData> CalculateInAdvanceRouteData(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;
	bool JudgeAttackable(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;//operatedUnit��aimedUnit�����̍s���ōU���ł��邩�ǂ������肷��
	std::vector<bool> JudgeAttackableList(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const std::vector<LogElement::UnitLogData> &aimedUnitList)const;//aimedUnit�������ɂ����g����
	std::vector<bool> JudgeAttackableList(const std::shared_ptr<LatticeBattleField> &latticeField,const LogElement::UnitLogData operatedUnit,const std::vector<LogElement::UnitLogData> &aimedUnitList)const;//�i�q�_�̐N���ۏ�񂾂��Ăяo�����Ŏ��R�Ɍ��߂���BbattleData���n���K�v���Ȃ��Ȃ�B
};

#endif // !DEF_SUBMISSIONRULEBASE_H
