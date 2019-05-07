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
	//�e��ʂɉ������X�R�A�̍X�V����
	void InitUpdate(const BattleSceneData * const battleData);
	void FinishUpdate(const BattleSceneData * const battleData);
	void AttackUpdate(const BattleSceneData * const battleData,const Unit * const aimedUnit);
	void ResearchUpdate();
	void WaitUpdate(const BattleSceneData * const battleData);
	void CancelUpdate();
	//�X�R�A�̌��ʏo�͏���
	std::string GetScoreExplain()const;
	//�R���X�g���N�^��
	ScoreObserver();
	virtual ~ScoreObserver();

private:

	std::vector<std::shared_ptr<LogElement>> m_logList;
	size_t m_researchCount;
	size_t m_cancelCount;
};

#endif // !DEF_SCOREOBSERVER_H