#ifndef DEF_SCOREOBSERVER_H
#define DEF_SCOREOBSERVER_H

#include<vector>
#include<memory>
#include<set>
#include"BattleSceneData.h"
#include"LogElement.h"

class ScoreObserver{
public:
	//�X�R�A�v�Z�̍���
	struct ScoreRule{
		//�ǂ̏�ʂōX�V���邩
		enum class UpdateScene{
			e_attack
			,e_research
			,e_wait
			,e_cancel
			,e_clear
		};
		explicit ScoreRule(const std::set<UpdateScene> &updateSceneSet):m_updateSceneSet(updateSceneSet){}
		virtual ~ScoreRule(){}
		bool JudgeUpdate(UpdateScene updateScene)const{
			return (m_updateSceneSet.find(updateScene)!=m_updateSceneSet.end());
		}
		//���̍��ڂ͂ǂ̏�ʂŃX�R�A�X�V���������邩(������ʂ�z��)
		const std::set<UpdateScene> m_updateSceneSet;
	};

	//�e��ʂɉ������X�R�A�̍X�V����
	void AttackUpdate(const BattleSceneData * const battleData,const Unit * const aimedUnit);
	void ResearchUpdate();
	void WaitUpdate(const BattleSceneData * const battleData);
	void CancelUpdate();
	void ClearUpdate(const BattleSceneData * const battleData);

private:
	ScoreObserver();
	virtual ~ScoreObserver();

	std::vector<std::shared_ptr<LogElement>> m_logList;
};

#endif // !DEF_SCOREOBSERVER_H