#ifndef DEF_SCOREOBSERVER_H
#define DEF_SCOREOBSERVER_H

#include<vector>
#include<memory>
#include<set>
#include"LogElement.h"
#include"StageLevel.h"
#include"SelfDecideSubmission.h"

//"BattleSceneData.h"��"ScoreSystem.h"�͏z�Q�ƂɂȂ��Ă��܂��̂Ő錾
struct BattleSceneData;

class ScoreObserver{
public:
	//�{�[�i�X�f�[�^�̌`��
	class Bonus{
	public:
		Bonus(const std::string &explain,int score);
		virtual ~Bonus();
		int GetScore()const{
			return m_score;
		}
		std::string GetExplain()const{
			return m_explain;
		}

	private:
		const std::string m_explain;
		const int m_score;
	};
	//�o�͂���X�R�A�̌`��
	struct ScoreExpression{
		ScoreExpression(const std::vector<Bonus> &bonusVec
			,size_t surviveCount
			,size_t unitCount
			,size_t defeatCount
			,size_t enemyCount
			,size_t turnCount
			,int totalScorePoint)
			:m_bonusVec(bonusVec)
			,m_surviveCount(surviveCount)
			,m_unitCount(unitCount)
			,m_defeatCount(defeatCount)
			,m_enemyCount(enemyCount)
			,m_turnCount(turnCount)
			,m_totalScorePoint(totalScorePoint)
		{}
		virtual ~ScoreExpression(){}

		std::vector<Bonus> m_bonusVec;//�{�[�i�X�ꗗ
		const size_t m_surviveCount;//������
		const size_t m_unitCount;//�����j�b�g��
		const size_t m_defeatCount;//���j��(�c�C�[�g�ɂ̂ݗp����)
		const size_t m_enemyCount;//�G���j�b�g��(�c�C�[�g�ɂ̂ݗp����)
		const size_t m_turnCount;//�^�[����
		const int m_totalScorePoint;//���v�X�R�A
	};

	//�e��ʂɉ������X�R�A�̍X�V����
	void InitUpdate(const BattleSceneData * const battleData);
	void FinishUpdate(const BattleSceneData * const battleData);
	void AttackUpdate(const BattleSceneData * const battleData,const Unit * const aimedUnit);
	void ResearchUpdate();
	void WaitUpdate(const BattleSceneData * const battleData);
	void CancelUpdate();
	//�X�R�A�̌��ʏo�͏���
	std::shared_ptr<ScoreExpression> GetScoreExpression(bool winFlag)const;
	//���߃��O���擾(�T�u�~�b�V�������ɗp����)
	std::shared_ptr<const LogElement> GetLatestLog()const;
	//�T�u�~�b�V�����̎Q�Ƃ��擾
	const SelfDecideSubmission &GetSubmission()const{
		return m_submission;
	}
	//�T�u�~�b�V�����̃��[����ݒ�
	void SetSubmissionRule(const std::shared_ptr<SubmissionRuleBase> &rule);
	//�R���X�g���N�^��
	ScoreObserver();
	virtual ~ScoreObserver();

private:
	class LatticeBonusData{
		//�i�q�_�֌W�̃{�[�i�X���쐬���邽�߂̃f�[�^�Ə���
	public:
		LatticeBonusData();
		~LatticeBonusData();
		void InputData(const BattleSceneData * const battleData,const LogElement::UnitLogData &operateUnitData);
		Bonus GetPlayerBonus()const;
		Bonus GetEnemyBonus()const;

	private:
		size_t m_playerCount;//�v���C���[�L�����̍��v�s����
		double m_totalPlayerRate;//�v���C���[�L�����ɂ��āu�����Ȃ�������s����i�q�_�̓����ۂɍs����i�q�_�̊����v�̍��v
		size_t m_enemyCount;//�G�L�����̍��v�s����
		double m_totalEnemyRate;
	};

	std::vector<std::shared_ptr<LogElement>> m_logList;
	//�T�u�~�b�V�����Ɋւ���f�[�^
	SelfDecideSubmission m_submission;
	//���O�Ƃ��Ďc���Ȃ����ׂȃf�[�^
	StageLevel m_stageLevel;
	size_t m_researchCount;
	size_t m_cancelCount;
	//�i�q�_�֌W�ɂ��Ă̓��O��p���Čv�Z����Ə��������߂��ɂȂ�̂ŁAUpdate�̂��тɃf�[�^���i�[����
	LatticeBonusData m_latticeBonusData;
};

#endif // !DEF_SCOREOBSERVER_H