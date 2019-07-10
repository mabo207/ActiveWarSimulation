#include"DxLib.h"
#include"AttackScene.h"
#include"GraphicControl.h"
#include"FilePath.h"

//-------------------AttackScene-------------------
const float AttackScene::moveLength=40.0;

AttackScene::AttackScene(const std::shared_ptr<BattleSceneData> &data,Unit *aimedUnit)
	:DamageScene(SceneKind::e_attackNormal
		,data
		,SceneKind::e_switch
		,aimedUnit
		,data->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(data->m_operateUnit,aimedUnit))
	,m_attackMotion({
		PositionControl(data->m_operateUnit->getPos(),data->m_operateUnit->getPos()+(m_aimedUnit->getPos()-data->m_operateUnit->getPos()).norm()*moveLength,motionFrame/2,Easing::TYPE_OUT,Easing::FUNCTION_LINER,1.0)
		,PositionControl(data->m_operateUnit->getPos()+(m_aimedUnit->getPos()-data->m_operateUnit->getPos()).norm()*moveLength,data->m_operateUnit->getPos(),motionFrame/2,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0)
	})
{
	//エフェクトの作成
	LoadDivGraph((FilePath::graphicDir+"effect/zangeki.png").c_str(),30,30,1,100,100,m_effect);
}

AttackScene::~AttackScene(){
	//エフェクトの解放
	for(size_t i=0;i<30;i++){
		DeleteGraph(m_effect[i]);
	}
}

void AttackScene::AnimationUpdate(){
	DamageScene::AnimationUpdate();//ダメージ文字のアニメーション更新
	m_attackMotion.Update();//攻撃ユニットのアニメーション更新
}

void AttackScene::DrawUnit()const{
	//ユニットの描画
	m_battleSceneData->DrawUnit(false,std::set<const Unit *>{m_battleSceneData->m_operateUnit,m_aimedUnit});
	//攻撃対象ユニットの描画
	if(m_aimedUnit!=nullptr){
		m_aimedUnit->DrawUnit(Vector2D(),m_battleSceneData->m_fpsMesuring.GetFrame(),false,false,false);
	}
	//攻撃を実行するユニットの描画
	m_battleSceneData->m_operateUnit->DrawUnit(Vector2D((float)m_attackMotion.GetX(),(float)m_attackMotion.GetY()),Vector2D(),m_battleSceneData->m_fpsMesuring.GetFrame(),false,false,false);
}

void AttackScene::DrawAnimation()const{
	//エフェクトの描画
	if(m_attackMotion.GetFrame()<30){
		//DrawGraph(((int)m_aimedUnit->getPos().x)-50,((int)m_aimedUnit->getPos().y)-50,m_effect[m_attackMotion.GetFrame()],TRUE);
	}
	//ダメージ文字の描画
	DamageScene::DrawAnimation();
}

bool AttackScene::JudgeAnimationEnd()const{
	return (DamageScene::JudgeAnimationEnd() && m_attackMotion.GetEndFlag());
}

void AttackScene::DamageProcess(){
	//コストの消費
	//攻撃実行時に攻撃分のOPを消費する仕様を変える場合は、波及箇所を考慮せよ（スコアの計算などはこの仕様を前提とした実装をしている）
	//m_battleSceneData->m_operateUnit->AddOP(m_battleSceneData->m_operateUnit->CalculateAddOPNormalAttack());
	m_battleSceneData->m_operateUnit->ConsumeOPByCost(m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetCost());
	//操作ユニット→対象ユニットへの攻撃処理
	DamageScene::DamageProcess();
	//対象ユニットが生き残っているなら反撃処理を行う
	//未実装
}

int AttackScene::UpdateNextScene(int index){
	//特に何もしない
	return index;
}

void AttackScene::ReturnProcess(){}
