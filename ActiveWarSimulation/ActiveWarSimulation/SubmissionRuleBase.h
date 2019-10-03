#ifndef DEF_SUBMISSIONRULEBASE_H
#define DEF_SUBMISSIONRULEBASE_H

#include<string>
#include<memory>
#include<vector>

#include"LogElement.h"
#include"LatticeBattleField.h"

struct BattleSceneData;
class Unit;

//�T�u�~�b�V�����]�����̃��[����\�����邽�߂̊��N���X
class SubmissionRuleBase{
public:
	virtual ~SubmissionRuleBase()=default;
	virtual int RubricEvaluate(const BattleSceneData * const battleData)const=0;//�P�̍s���ɂ��ă��[�u���b�N�]��������
	virtual std::pair<std::string,unsigned int> GetRubricStringInfo(int rubric)const=0;//���[�u���b�N�]���ɑ΂��镶�����`�悷�邽�߂̏���Ԃ��i�����񁕕����̉��̐F�j
	virtual std::string GetWholeLookBack(int mostFrequentEvaluate)const=0;//�ł������������[�u���b�N�]���ɂ��āA�����I�ɂȃR�����g��Ԃ�
	virtual std::string GetWholeLookBackActionEmpty()const=0;//���[���ɊY�������s���������Ȃ��������̑����I�]���̕������Ԃ�
	virtual std::string GetSubmissionExplanation()const=0;//�T�u�~�b�V�����̐�������Ԃ�
	virtual bool JudgeEvaluateOrder(const BattleSceneData * const battleData)const=0;//�]�����s�����ǂ���

protected:
	SubmissionRuleBase()=default;
	//�悭�]���ɗp����������v�Z����֐�
	float CalculateRouteDistance(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit *operatedUnit,const Unit *aimedUnit)const;//operatedUnit����aimedUnit�܂ł̌o�H�������v�Z����
	bool JudgeAttackable(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit * operatedUnit,const Unit *aimedUnit)const;//operatedUnit��aimedUnit�����̍s���ōU���ł��邩�ǂ������肷��
	std::vector<bool> JudgeAttackableList(const BattleSceneData * const battleData,const std::vector<LogElement::UnitLogData> &unitDataList,const Unit * operatedUnit,const std::vector<const Unit *> &aimedUnitList)const;//aimedUnit�������ɂ����g����
	std::vector<bool> JudgeAttackableList(const std::shared_ptr<LatticeBattleField> &latticeField,const Unit * operatedUnit,const std::vector<const Unit *> &aimedUnitList)const;//�i�q�_�̐N���ۏ�񂾂��Ăяo�����Ŏ��R�Ɍ��߂���BbattleData���n���K�v���Ȃ��Ȃ�B
};

#endif // !DEF_SUBMISSIONRULEBASE_H
