#ifndef DEF_SCOREOBSERVER_H
#define DEF_SCOREOBSERVER_H

#include<vector>
#include<memory>
#include<set>
#include"LogElement.h"

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
			,size_t turnCount
			,int totalScorePoint)
			:m_bonusVec(bonusVec)
			,m_surviveCount(surviveCount)
			,m_unitCount(unitCount)
			,m_turnCount(turnCount)
			,m_totalScorePoint(totalScorePoint)
		{}
		virtual ~ScoreExpression(){}

		std::vector<Bonus> m_bonusVec;//�{�[�i�X�ꗗ
		const int m_surviveCount;//������
		const int m_unitCount;//�����j�b�g��
		const int m_turnCount;//�^�[����
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
	std::shared_ptr<ScoreExpression> GetScoreExpression()const;
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
	//���O�Ƃ��Ďc���Ȃ����ׂȃf�[�^
	size_t m_researchCount;
	size_t m_cancelCount;
	//�i�q�_�֌W�ɂ��Ă̓��O��p���Čv�Z����Ə��������߂��ɂȂ�̂ŁAUpdate�̂��тɃf�[�^���i�[����
	LatticeBonusData m_latticeBonusData;
};

#endif // !DEF_SCOREOBSERVER_H