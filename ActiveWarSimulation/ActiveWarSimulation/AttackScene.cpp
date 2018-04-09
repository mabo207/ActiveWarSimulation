#include"DxLib.h"
#include"AttackScene.h"
#include"GraphicControl.h"

//-------------------AttackScene-------------------
const int AttackScene::motionFlame=120;
const int AttackScene::damageBeginFlame=50;
const int AttackScene::damageEndFlame=80;

AttackScene::AttackScene(std::shared_ptr<BattleSceneData> data,Unit *aimedUnit)
	:BattleSceneElement(SceneKind::e_attackNormal)
	,m_battleSceneData(data),m_aimedUnit(aimedUnit)
	,m_attackMotion({
		PositionControl(data->m_operateUnit->getPos(),m_aimedUnit->getPos(),motionFlame/2,Easing::TYPE_OUT,Easing::FUNCTION_QUAD,1.0)
		,PositionControl(m_aimedUnit->getPos(),data->m_operateUnit->getPos(),motionFlame/2,Easing::TYPE_IN,Easing::FUNCTION_QUAD,1.0)
	})
	,m_damageMotion(0,0,0,-(int)Unit::unitCircleSize,damageEndFlame-damageBeginFlame,Easing::TYPE_OUT,Easing::FUNCTION_QUAD,9.0)
{
	//攻撃情報の記録
	m_attackinfo=m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_battleSceneData->m_operateUnit,m_aimedUnit);
	m_damageFont=CreateFontToHandleEX("メイリオ",30,4,DX_FONTTYPE_ANTIALIASING_EDGE);
}

AttackScene::~AttackScene(){
	DeleteFontToHandleEX(m_damageFont);
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

	}
	//終了判定
	if(m_attackMotion.GetEndFlag() && m_damageMotion.GetEndFlag()){
		//ひとまず直前場面へ
		return -1;
	}

	return SceneKind::e_attackNormal;
}

void AttackScene::thisDraw()const{
	//フィールドの描画
	m_battleSceneData->DrawField(std::set<const BattleObject *>{m_battleSceneData->m_operateUnit,m_aimedUnit});

	//攻撃対象ユニットの描画
	if(m_aimedUnit!=nullptr){
		m_aimedUnit->BattleObject::VDraw();
	}
	
	//攻撃を実行するユニットの描画
	m_battleSceneData->m_operateUnit->VDraw(Vector2D((float)m_attackMotion.GetX(),(float)m_attackMotion.GetY()),Vector2D());
	
	//全ユニットのHPゲージの描画
	m_battleSceneData->DrawHPGage();

	//ユニットのオーダー順番を描画
	m_battleSceneData->DrawOrder();

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
