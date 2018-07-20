#include"DxLib.h"
#include"MoveScene.h"
#include"AttackScene.h"
#include"ResearchScene.h"
#include"input.h"
#include"Edge.h"
#include<algorithm>
#include"GraphicControl.h"

//----------------------MoveScene----------------------
const float MoveScene::routeFrequency=1.0f;

MoveScene::MoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_move),m_battleSceneData(battleSceneData)
{
	LoadDivGraphEX("Graphic/attackedCursor.png",attackedCursorPicNum,attackedCursorPicNum,1,40,44,m_attackedCursor);
	//m_aimedUnit等の初期化
	FinishUnitOperation();
}

MoveScene::~MoveScene(){
	for(size_t i=0;i<attackedCursorPicNum;i++){
		DeleteGraphEX(m_attackedCursor[i]);
	}
}

bool MoveScene::PositionUpdate(const Vector2D inputVec){
	//バトルデータの更新
	bool inputFlag=m_battleSceneData->PositionUpdate(inputVec);

	//攻撃対象ユニットの更新(移動しなくても(=inputFlagがfalseでも)ユニットの位置は動く可能性があるので毎ループ処理する)
	bool changeAimedUnitFlag;//対象ユニットの変更を行うか
	float aimedUnitAngle;//対象ユニットのいた方向
	if(m_aimedUnit!=nullptr){
		//前のフレームで対象ユニットがいた場合
		if(m_battleSceneData->m_operateUnit->JudgeAttackable(m_aimedUnit)){
			//現在の対象ユニットが操作ユニットの攻撃範囲に入っている時は特に何もしない
			changeAimedUnitFlag=false;
		} else{
			//現在の対象ユニットが操作ユニットの攻撃範囲に入っていない時は
			changeAimedUnitFlag=true;
			aimedUnitAngle=(m_aimedUnit->getPos()-m_battleSceneData->m_operateUnit->getPos()).GetRadian();
		}
	} else{
		//前のフレームで対象ユニットがいなかった場合
		changeAimedUnitFlag=true;
		aimedUnitAngle=0.0f;
	}
	if(changeAimedUnitFlag){
		//対象ユニットの変更
		SetAimedUnit(aimedUnitAngle,0);
	}
	//m_routeの追加
	if(m_route.empty() || m_route.back().OP-m_battleSceneData->m_operateUnit->GetBattleStatus().OP>routeFrequency){
		//routeFrequencyを以前の計測以降に消費したOPが越したら現在位置と現在のOPをm_routeに格納
		m_route.push_back(RouteInfo(m_battleSceneData->m_operateUnit->getPos(),m_battleSceneData->m_operateUnit->GetBattleStatus().OP));
	}

	return inputFlag;
}

void MoveScene::FinishUnitOperation(){
	//バトルデータの更新
	//m_battleSceneData->FinishUnitOperation();
	//m_aimedUnitの初期化
	SetAimedUnit(0.0f,0);
	//m_routeの初期化
	m_route.clear();

}

void MoveScene::SetAimedUnit(float angle,int turntimes){
	//範囲内のユニット一覧の作成
	std::vector<Unit *> list;
	for(Unit *pUnit:m_battleSceneData->m_unitList){
		if(m_battleSceneData->m_operateUnit->JudgeAttackable(pUnit)){
			//異なるチームかつ一定距離内にいれば追加
			list.push_back(pUnit);
		}
	}
	//攻撃対象ユニットを設定
	m_aimedUnit=nullptr;//ひとまずnullptrに
	if(!list.empty()){
		//比較関数の作成と並べ替え。m_operateUnitからのベクトルの向きでソートする
		const Vector2D pos=m_battleSceneData->m_operateUnit->getPos();
		auto f=[&pos](Unit *lobj,Unit *robj)->bool{
			const float langle=(lobj->getPos()-pos).GetRadian(),rangle=(robj->getPos()-pos).GetRadian();
			return langle<rangle;
		};
		std::sort(list.begin(),list.end(),f);
		//aimedUnitAngleに時計回り方向に近いユニットを探す(turntimesが1であるユニットの検出)
		std::vector<Unit *>::const_iterator it=list.begin(),ite=list.end();
		for(;it!=ite;it++){
			if(angle<((*it)->getPos()-pos).GetRadian()){
				//初めてaimedUnitAngleを超えた所にいるオブジェクトを初期オブジェクトとする
				break;
			}
		}
		if(it==list.end()){
			it=list.begin();
		}
		//turntimes回の回転に合わせる。現時点ではturntimes=1のユニットが求められている。
		if(turntimes>0){
			//正の方向の回転は、iteに達したらlist.begin()に戻す事だけ気をつければ良い
			for(int i=1;i<turntimes;i++){
				it++;
				if(it==ite){
					it=list.begin();
				}
			}
		} else{
			//0以下の回転は、angleにぴったり合わさった場合とそうでない場合で異なる
			std::vector<Unit *>::const_iterator beforeit=it;//turntimes=1の直前のイテレータ
			if(beforeit==list.begin()){
				beforeit=list.end();
			}
			beforeit--;
			//itをturntimes=0の場所に合わせる
			if(angle==((*beforeit)->getPos()-pos).GetRadian()){
				//angleにぴったり合わさったユニットが存在する場合
				it=beforeit;//itをturntimes=1のユニットより1つ前に戻す
			}
			//turntimes回の回転に合わせる
			for(int i=0;i>turntimes;i--){
				if(it==list.begin()){
					it=list.end();
				}
				it--;
			}
		}
		//求めたイテレータに格納されているユニットをm_aimedUnitに格納する
		m_aimedUnit=*it;
	}
}

bool MoveScene::JudgeAttackCommandUsable()const{
	return m_aimedUnit!=nullptr && m_battleSceneData->m_operateUnit->ConsumeOPVirtualByCost(m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetCost())>=0.0f;
}

/*
int MoveScene::thisCalculate(){
	if(m_battleSceneData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
		//味方操作時
		//m_operateUnitの位置更新
		if(PositionUpdate(CalculateInputVec())){
			//位置更新をした時の処理

		} else{
			//移動操作をしなかった時はその他の入力を受け付ける
			if(keyboard_get(KEY_INPUT_Z)==1){
				//攻撃
				if(JudgeAttackCommandUsable()){
					//攻撃対象が存在し、OPが足りている場合のみ攻撃処理を行う
					//FinishUnitOperation();//行動終了処理(あとで)
					return SceneKind::e_attackNormal;//攻撃場面へ
				}
			} else if(keyboard_get(KEY_INPUT_D)==1){
				//必殺技

			} else if(keyboard_get(KEY_INPUT_A)==1 && JudgeAttackCommandUsable()){
				//攻撃コマンド使用可能の時のみ、狙いのキャラの変更(反時計回り)
				float angle;
				if(m_aimedUnit!=nullptr){
					angle=(m_aimedUnit->getPos()-m_battleSceneData->m_operateUnit->getPos()).GetRadian();
				} else{
					angle=0.0f;
				}
				SetAimedUnit(angle,-1);
			} else if(keyboard_get(KEY_INPUT_S)==1 && JudgeAttackCommandUsable()){
				//攻撃コマンド使用可能の時のみ、狙いのキャラの変更(時計回り)
				float angle;
				if(m_aimedUnit!=nullptr){
					angle=(m_aimedUnit->getPos()-m_battleSceneData->m_operateUnit->getPos()).GetRadian();
				} else{
					angle=0.0f;
				}
				SetAimedUnit(angle,1);
			} else if(keyboard_get(KEY_INPUT_C)==1){
				//アイテムの使用

			} else if(keyboard_get(KEY_INPUT_V)==1){
				//待機
				FinishUnitOperation();
				return 0;
			} else if(keyboard_get(KEY_INPUT_X)==1 || keyboard_get(KEY_INPUT_X)>30){
				//移動やり直し(m_route.back()の1つ前の場所に戻す。back()の位置は現在位置の可能性が高いため)
				if(!m_route.empty()){
					m_route.pop_back();
					if(!m_route.empty()){
						//もう要素がなければpop_back()をしない
						RouteInfo info=m_route.back();
						m_route.pop_back();
						//ユニットを移動させる
						m_battleSceneData->m_operateUnit->Warp(info.pos);
						//OPを回復させる
						m_battleSceneData->m_operateUnit->AddOP(info.OP-m_battleSceneData->m_operateUnit->GetBattleStatus().OP);
					}
					//位置更新を行う
					PositionUpdate(Vector2D());
				}
			}
		}
	} else{
		//敵操作時
		//m_operateUnitの位置更新
		const Vector2D beforeVec=m_battleSceneData->m_operateUnit->getPos();
		PositionUpdate(CalculateInputVec());
		const float moveSqLength=(beforeVec-m_battleSceneData->m_operateUnit->getPos()).sqSize();
		const double processedTime=m_battleSceneData->m_fpsMesuring.GetProcessedTime();
		if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0){
			//1秒経ったら行動する
			if(JudgeAttackCommandUsable()){
				//攻撃対象が存在し、OPが足りている場合のみ攻撃処理を行う
				//FinishUnitOperation();//行動終了処理(あとで)
				return SceneKind::e_attackNormal;//攻撃場面へ
			} else if(m_battleSceneData->m_operateUnit->GetBattleStatus().OP<2.0f || processedTime>10.0 || (moveSqLength<0.1f && processedTime>2.0)){
				//移動できなくなったら、または10秒経ったら、また移動距離が少ない場合は待機
				FinishUnitOperation();
				return 0;
			}
		}
	}
	return SceneKind::e_move;
}
//*/

void MoveScene::thisDraw()const{
	//経路の描画
	for(size_t i=0,max=m_route.size();i+1<max;i++){
		DrawLineAA(m_route[i].pos.x,m_route[i].pos.y,m_route[i+1].pos.x,m_route[i+1].pos.y,GetColor(255,255,0),1.0f);
	}

	//フィールドの描画
	m_battleSceneData->DrawField();

	//ユニットの描画
	m_battleSceneData->DrawUnit(true,std::set<const Unit *>{m_battleSceneData->m_operateUnit,m_aimedUnit});

	//狙っているユニットの描画
	if(m_aimedUnit!=nullptr){
		m_aimedUnit->BattleObject::VDraw();
	}

	//操作中ユニットの描画
	m_battleSceneData->m_operateUnit->BattleObject::VDraw();
	m_battleSceneData->m_operateUnit->DrawMoveInfo();//移動情報の描画


	//全ユニットのHPゲージの描画
	m_battleSceneData->DrawHPGage();

	//アイコン等を描く
	Vector2D pos;
	//狙っているユニット
	if(m_aimedUnit!=nullptr){
		if(JudgeAttackCommandUsable()){
			//攻撃可能ならマーカーを描画
			pos=m_aimedUnit->getPos();
			//DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(0,255,0),TRUE);
			size_t index=(m_battleSceneData->m_fpsMesuring.GetFlame()/15)%attackedCursorPicNum;
			float dx,dy;
			GetGraphSizeF(m_attackedCursor[index],&dx,&dy);
			DrawGraph((int)(pos.x-dx/2.0f),(int)(pos.y-dy-Unit::unitCircleSize+10.0f),m_attackedCursor[index],TRUE);
		}
	}
	//操作ユニット
	pos=m_battleSceneData->m_operateUnit->getPos();
	DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(255,255,0),TRUE);


	//ユニットのオーダー順番を描画
	m_battleSceneData->DrawOrder();

}

int MoveScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_attackNormal):
		m_nextScene=std::shared_ptr<BattleSceneElement>(new AttackScene(m_battleSceneData,m_aimedUnit));
		return index;
	case(SceneKind::e_research):
		m_nextScene=std::shared_ptr<BattleSceneElement>(new ResearchScene(m_battleSceneData));
		return index;
	default:
		return index;
	}
}

void MoveScene::ReturnProcess(){
	//特に何もしない
}
