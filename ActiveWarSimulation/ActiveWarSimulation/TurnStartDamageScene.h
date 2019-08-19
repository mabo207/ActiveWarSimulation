#ifndef DEF_TURNSTARTDAMAGESCENE_H
#define DEF_TURNSTARTDAMAGESCENE_H

#include"DamageScene.h"

//移動の始めにダメージを喰らう・回復をする場面
class TurnStartDamageScene:public DamageScene{
public:
	TurnStartDamageScene(const std::shared_ptr<BattleSceneData> &data);
	~TurnStartDamageScene();
	
	static bool JudgeTurnStartDamage(const std::shared_ptr<BattleSceneData> &data);//ターンの始めにダメージ処理を行う条件でないなら、スキップしてMoveSceneに行きたい
	static Weapon::AttackInfo GetAttackInfo(const std::shared_ptr<BattleSceneData> &data);

protected:
	void DrawAnimation()const;//エフェクトを表示する必要がある
	void DamageProcess();//m_aimedUnitが死んだ場合、遷移先を変える必要がある
	int UpdateNextScene(int index);
	void ReturnProcess();

	int m_effect[30];
};

#endif // !DEF_TURNSTARTDAMAGESCENE_H
