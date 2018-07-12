#include"DxLib.h"
#include"AttackScene.h"
#include"GraphicControl.h"

//-------------------AttackScene-------------------
const int AttackScene::motionFlame=20;
const int AttackScene::damageBeginFlame=AttackScene::motionFlame/2;
const int AttackScene::damageEndFlame=AttackScene::damageBeginFlame+30;
const float AttackScene::moveLength=40.0;

AttackScene::AttackScene(std::shared_ptr<BattleSceneData> data,Unit *aimedUnit)
	:BattleSceneElement(SceneKind::e_attackNormal)
	,m_battleSceneData(data),m_aimedUnit(aimedUnit)
	,m_attackMotion({
		PositionControl(data->m_operateUnit->getPos(),data->m_operateUnit->getPos()+(m_aimedUnit->getPos()-data->m_operateUnit->getPos()).norm()*moveLength,motionFlame/2,Easing::TYPE_OUT,Easing::FUNCTION_LINER,1.0)
		,PositionControl(data->m_operateUnit->getPos()+(m_aimedUnit->getPos()-data->m_operateUnit->getPos()).norm()*moveLength,data->m_operateUnit->getPos(),motionFlame/2,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0)
	})
	,m_damageMotion({
		PositionControl(0,0,0,-(int)Unit::unitCircleSize,(damageEndFlame-damageBeginFlame)/2,Easing::TYPE_OUT,Easing::FUNCTION_EXPO,9.0)
		,PositionControl(0,0,-(int)Unit::unitCircleSize,-(int)Unit::unitCircleSize,(damageEndFlame-damageBeginFlame)/2,Easing::TYPE_OUT,Easing::FUNCTION_QUAD,9.0)
	})
{
	//攻撃情報の記録
	m_attackinfo=m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_battleSceneData->m_operateUnit,m_aimedUnit);
	m_damageFont=CreateFontToHandleEX("メイリオ",30,4,DX_FONTTYPE_ANTIALIASING_EDGE);
}

AttackScene::~AttackScene(){
	DeleteFontToHandleEX(m_damageFont);
}

void AttackScene::ProcessAttack(){
	//コストの消費
	//m_battleSceneData->m_operateUnit->AddOP(m_battleSceneData->m_operateUnit->CalculateAddOPNormalAttack());
	m_battleSceneData->m_operateUnit->ConsumeOPByCost(m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetCost());
	//操作ユニット→対象ユニットへの攻撃情報の計算
	Weapon::AttackInfo attackinfo=m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_battleSceneData->m_operateUnit,m_aimedUnit);
	//操作ユニット→対象ユニットへの攻撃処理
	int aimedHP=m_aimedUnit->AddHP(-attackinfo.damage);
	if(aimedHP<=0){
		//対象ユニットのHPが0以下なら、ステージからユニットを取り除く
		m_aimedUnit->SetFix(Shape::Fix::e_ignore);//当たり判定の対象から取り除く
		//m_unitListからm_aimedUnitを取り除く
		for(std::vector<Unit *>::const_iterator it=m_battleSceneData->m_unitList.begin(),ite=m_battleSceneData->m_unitList.end();it!=ite;it++){
			if(*it==m_aimedUnit){
				m_battleSceneData->m_unitList.erase(it);
				break;
			}
		}
	} else{
		//対象ユニットが生き残っているなら反撃処理を行う
		//未実装
	}
}

int AttackScene::thisCalculate(){
	//アニメーション更新
	m_attackMotion.Update();
	m_damageMotion.Update();
	const int flame=m_attackMotion.GetFlame();
	//ダメージモーションの開始
	if(flame==damageBeginFlame){
		//アニメーションの初期化
		m_damageMotion.Retry();
		//ダメージ処理
		ProcessAttack();
	}
	//終了判定
	if(m_attackMotion.GetEndFlag() && m_damageMotion.GetEndFlag()){
		//ユニット切り替え場面へ
		return SceneKind::e_switch;
	}

	return SceneKind::e_attackNormal;
}

void AttackScene::thisDraw()const{
	//フィールドの描画
	m_battleSceneData->DrawField();

	//ユニットの描画
	m_battleSceneData->DrawUnit(false,std::set<const Unit *>{m_battleSceneData->m_operateUnit,m_aimedUnit});

	//攻撃対象ユニットの描画
	if(m_aimedUnit!=nullptr){
		m_aimedUnit->DrawUnit(m_aimedUnit->getPos(),Vector2D(),false);
	}
	
	//攻撃を実行するユニットの描画
	m_battleSceneData->m_operateUnit->DrawUnit(Vector2D((float)m_attackMotion.GetX(),(float)m_attackMotion.GetY()),Vector2D(),false);
	
	//全ユニットのHPゲージの描画
	m_battleSceneData->DrawHPGage();

	//ダメージの描画
	if(m_attackMotion.GetFlame()>damageBeginFlame){
		//表示するフレームより前は描画処理を行わない
		//色の決定
		unsigned int incolor,outcolor;
		if(m_attackinfo.damage<=0){
			//ノーダメージや回復の場合は緑で描画
			incolor=GetColor(64,255,64);
			outcolor=GetColor(0,0,0);
		} else{
			//ダメージは赤く描画
			incolor=GetColor(255,0,0);
			outcolor=GetColor(255,255,255);
		}
		//描画
		const int x=(int)m_aimedUnit->getPos().x+m_damageMotion.GetX(),y=(int)m_aimedUnit->getPos().y+m_damageMotion.GetY();
		DrawStringCenterBaseToHandle(x,y,std::to_string(std::abs(m_attackinfo.damage)).c_str(),incolor,m_damageFont,true,outcolor);
	}
}

int AttackScene::UpdateNextScene(int index){
	//特に何もしない
	return index;
}

void AttackScene::ReturnProcess(){}
