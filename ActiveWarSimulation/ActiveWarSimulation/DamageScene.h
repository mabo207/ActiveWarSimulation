#ifndef DEF_DAMAGESCENE_H
#define DEF_DAMAGESCENE_H

#include"BattleSceneElement.h"
#include"ToolsLib.h"
#include"Unit.h"
#include"BattleSceneData.h"

//自分のHPが変わるバトル場面の基底クラス
class DamageScene:public BattleSceneElement{
public:
	DamageScene(SceneKind::Kind sceneKind
		,const std::shared_ptr<BattleSceneData> &data
		,SceneKind::Kind nextSceneKind
		,Unit *aimedUnit
		,const Weapon::AttackInfo &attackInfo);
	virtual ~DamageScene();

protected:
	//仮想関数のオーバーライド
	int thisCalculate();
	void thisDraw()const;
	//継承先で異なる
	virtual void AnimationUpdate();
	virtual void DrawUnit()const;//ダメージ内容によってユニットの描画方法が異なる
	virtual void DrawAnimation()const;//アニメーションさせるものが複数存在する可能性があるため。
	virtual bool JudgeAnimationEnd()const;
	virtual void DamageProcess();//デフォルトはm_aimedUnitがダメージを受けるだけ

	//ゲームのデータ
	std::shared_ptr<BattleSceneData> m_battleSceneData;
	//次のシーンが何か
	const SceneKind::Kind m_nextSceneKind;
	//攻撃されるユニットのデータ
	Unit *const m_aimedUnit;
	//経過フレーム数
	int m_frame;
	//位置変更データ
	PositionComplexControl m_damageMotion;//ダメージ文字
	//攻撃情報についてのデータ
	const Weapon::AttackInfo m_attackinfo;//攻撃情報
	const int m_damageFont;//ダメージ描画に用いるフォント

	static const int motionFrame;//アニメーションする全体時間
	static const int damageBeginFrame,damageEndFrame;//ダメージ文字が出現するタイミング、アニメーションが終わるタイミング
};


#endif // !DEF_DAMAGESCENE_H
