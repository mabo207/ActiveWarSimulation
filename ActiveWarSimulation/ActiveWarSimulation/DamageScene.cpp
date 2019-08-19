#include"DamageScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"FilePath.h"

//-----------------DamageScene----------------
const int DamageScene::motionFrame=20;
const int DamageScene::damageBeginFrame=DamageScene::motionFrame/2;
const int DamageScene::damageEndFrame=DamageScene::damageBeginFrame+30;

DamageScene::DamageScene(SceneKind::Kind sceneKind
	,const std::shared_ptr<BattleSceneData> &data
	,SceneKind::Kind nextSceneKind
	,Unit *aimedUnit
	,const Weapon::AttackInfo &attackInfo)
	:BattleSceneElement(sceneKind)
	,m_battleSceneData(data)
	,m_nextSceneKind(nextSceneKind)
	,m_aimedUnit(aimedUnit)
	,m_frame(0)
	,m_damageMotion({
		PositionControl(0,0,0,-(int)Unit::unitCircleSize,(damageEndFrame-damageBeginFrame)/2,Easing::TYPE_OUT,Easing::FUNCTION_EXPO,9.0)
		,PositionControl(0,0,-(int)Unit::unitCircleSize,-(int)Unit::unitCircleSize,(damageEndFrame-damageBeginFrame)/2,Easing::TYPE_OUT,Easing::FUNCTION_QUAD,9.0)
	})
	,m_attackinfo(attackInfo)
	,m_damageFont(LoadFontDataToHandleEX(FilePath::fontDir+"LargeThickGothicFont.dft",1))
{}

DamageScene::~DamageScene(){
	//フォントの解放
	DeleteFontToHandleEX(m_damageFont);
}

int DamageScene::thisCalculate(){
	//アニメーション更新
	AnimationUpdate();
	m_frame++;
	//ダメージモーションの開始
	if(m_frame==damageBeginFrame){
		//ダメージ文字アニメーションの初期化
		m_damageMotion.Retry();
		//ダメージ処理
		DamageProcess();
	}
	//終了判定
	if(JudgeAnimationEnd()){
		//ユニット切り替え場面へ
		return m_nextSceneKind;
	}

	return GetSceneKind();
}

void DamageScene::thisDraw()const{
	//フィールドの描画
	m_battleSceneData->DrawField();
	//ユニットの描画
	DrawUnit();
	//全ユニットのHPゲージの描画
	m_battleSceneData->DrawHPGage();
	//アニメーションの描画
	DrawAnimation();
}

void DamageScene::AnimationUpdate(){
	//ダメージアニメーションの更新
	m_damageMotion.Update();
}

void DamageScene::DrawUnit()const{
	//ダメージ対象ユニット以外の描画
	m_battleSceneData->DrawUnit(false,std::set<const Unit *>{m_aimedUnit});

	//ダメージ対象ユニットの描画
	if(m_aimedUnit!=nullptr){
		m_aimedUnit->DrawUnit(Vector2D(),m_battleSceneData->m_fpsMesuring.GetFrame(),false,false,false);
	}
}

void DamageScene::DrawAnimation()const{
	//デフォルトはダメージの描画のみ行う
	if(m_frame>damageBeginFrame){
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

bool DamageScene::JudgeAnimationEnd()const{
	//デフォルトは、m_damageMotionの開始と終了のみを見る
	return (m_frame>=damageBeginFrame && m_damageMotion.GetEndFlag());
}

void DamageScene::DamageProcess(){
	//デフォルトは、m_aimedUnitがどうなったかだけを見る
	//操作ユニット→対象ユニットへの攻撃処理
	const int aimedHP=m_aimedUnit->AddHP(-m_attackinfo.damage);
	//効果音を鳴らす
	if(m_attackinfo.damage>0){
		//ダメージがあれば打撃音
		PlaySoundMem(m_battleSceneData->m_attackSound,DX_PLAYTYPE_BACK,TRUE);
	} else{
		//回復しているなら回復音
		PlaySoundMem(m_battleSceneData->m_healSound,DX_PLAYTYPE_BACK,TRUE);
	}
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
	}
}
