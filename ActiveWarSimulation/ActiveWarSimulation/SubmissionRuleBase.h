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
	struct InAdvanceCalculateData{
		//���O�v�Z�������ʂ��i�[����N���X�A�p�����ėp����
	};

	virtual ~SubmissionRuleBase()=default;
	SubmissionEvaluation RubricEvaluate(const BattleSceneData * const battleData)const;//�P�̍s���ɂ��ă��[�u���b�N�]��������
	virtual SubmissionEvaluation RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const=0;
	virtual SubmissionEvaluation InAdvanceDataEvaluate(const std::shared_ptr<InAdvanceCalculateData> &,const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
		//���O�v�Z�f�[�^��p���č����]������֐�
		//�f�t�H���g�ł́A�ʏ�]�����s���悤�ɂȂ��Ă���
		return RubricEvaluate(field,stageSize,evaluateLog);
	}
	virtual std::string GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const=0;//�ł������������[�u���b�N�]���ɂ��āA�����I�ɂȃR�����g��Ԃ�
	virtual std::string GetWholeLookBackActionEmpty()const=0;//���[���ɊY�������s���������Ȃ��������̑����I�]���̕������Ԃ�
	virtual std::string GetSubmissionExplanation()const=0;//�T�u�~�b�V�����̐�������Ԃ�
	virtual bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const=0;//�]�����s�����ǂ���
	virtual std::string GetReason(SubmissionEvaluation rubric)const=0;//���[�u���b�N�]���̗��R�̕��͂�Ԃ�

protected:
	SubmissionRuleBase()=default;
	//�悭�]���ɗp����������v�Z����֐��AUnit *�łȂ�UnitLogData��p���Čv�Z�����Ȃ��Ƃ����Ȃ�
	float CalculateRouteDistance(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;//operatedUnit����aimedUnit�܂ł̌o�H�������v�Z����
	bool JudgeAttackable(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;//operatedUnit��aimedUnit�����̍s���ōU���ł��邩�ǂ������肷��
	std::vector<bool> JudgeAttackableList(const std::vector<BattleObject *> &field,const Vector2D mapSize,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const std::vector<LogElement::UnitLogData> &aimedUnitList)const;//aimedUnit�������ɂ����g����
	std::vector<bool> JudgeAttackableList(const std::shared_ptr<LatticeBattleField> &latticeField,const LogElement::UnitLogData operatedUnit,const std::vector<LogElement::UnitLogData> &aimedUnitList)const;//�i�q�_�̐N���ۏ�񂾂��Ăяo�����Ŏ��R�Ɍ��߂���BbattleData���n���K�v���Ȃ��Ȃ�B
	//BattleSceneData��p�������
	float CalculateRouteDistance(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;//operatedUnit����aimedUnit�܂ł̌o�H�������v�Z����
	bool JudgeAttackable(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const LogElement::UnitLogData aimedUnit)const;//operatedUnit��aimedUnit�����̍s���ōU���ł��邩�ǂ������肷��
	std::vector<bool> JudgeAttackableList(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const LogElement::UnitLogData operatedUnit,const std::vector<LogElement::UnitLogData> &aimedUnitList)const;//aimedUnit�������ɂ����g����
};

#endif // !DEF_SUBMISSIONRULEBASE_H
