#define _USE_MATH_DEFINES
#include"DxLib.h"
#include"TutorialPlayerMoveScene.h"
#include"Circle.h"
#include"GeneralPurposeResourceManager.h"
#include<cmath>
#include"CommonConstParameter.h"

//-------------------TutorialPlayerMoveScene---------------
TutorialPlayerMoveScene::TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:PlayerMoveScene(battleSceneData)
	,m_tutorialBattleSceneData(std::dynamic_pointer_cast<TutorialBattleSceneData>(battleSceneData))
	,m_animeFlame(0)
{
	//先頭のチュートリアル項目に従ってbool値を更新
	UpdateFlagOnlyInherit();
}

TutorialPlayerMoveScene::~TutorialPlayerMoveScene(){}

void TutorialPlayerMoveScene::UpdateFlagOnlyInherit(){
	if(!m_tutorialBattleSceneData->m_tutorialData.empty()){
		//チュートリアル項目が残っている場合、m_tutorialBattleSceneDataを用いて出来る動作を制限
		//まず全てできなくさせる
		m_moveableOnlyChangeInherit=false;
		m_attackableOnlyChangeInherit=false;
		m_waitableOnlyChangeInherit=false;
		//次に、出来る行動を設定する
		switch(m_tutorialBattleSceneData->m_tutorialData[0]->m_kind){
		case(TutorialBattleSceneData::TutorialBase::TutorialKind::e_move):
			m_moveableOnlyChangeInherit=true;
			break;
		case(TutorialBattleSceneData::TutorialBase::TutorialKind::e_normalAttack):
			m_attackableOnlyChangeInherit=true;
			break;
		case(TutorialBattleSceneData::TutorialBase::TutorialKind::e_wait):
			m_waitableOnlyChangeInherit=true;
			break;
		case(TutorialBattleSceneData::TutorialBase::TutorialKind::e_explain):
			break;
		default:
			break;
		}
	} else{
		//チュートリアル項目が残っていない場合、何でもできるようにする
		m_moveableOnlyChangeInherit=true;
		m_attackableOnlyChangeInherit=true;
		m_waitableOnlyChangeInherit=true;
	}
}

void TutorialPlayerMoveScene::GoNextTutorial(){
	//先頭のチュートリアルデータを取り除く
	m_tutorialBattleSceneData->m_tutorialData.erase(m_tutorialBattleSceneData->m_tutorialData.begin());
	//フラグの更新
	UpdateFlagOnlyInherit();
}

bool TutorialPlayerMoveScene::TutorialMoveProcess(int retIntPal){
	if(retIntPal==SceneKind::e_move){
		//移動をしたことを確認した時のみ、キャストする。
		const TutorialBattleSceneData::MoveTutorial *data=dynamic_cast<const TutorialBattleSceneData::MoveTutorial *>(m_tutorialBattleSceneData->m_tutorialData[0].get());
		if(data!=nullptr
			&& data->m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())
			)
		{
			//目的地に移動できたので、次のチュートリアルデータへ更新
			GoNextTutorial();
		}
		return false;//移動が行われた時は常に遷移処理を行わない
	} else{
		//移動以外の動作がされている場合は、遷移処理をする(マップを調べるなど)
		return true;
	}
}

bool TutorialPlayerMoveScene::TutorialAttackProcess(int retIntPal){
	if(retIntPal==SceneKind::e_attackNormal){
		//攻撃を実行しようとした入力を検知した時のみ、キャストする
		const TutorialBattleSceneData::AttackTutorial *data=dynamic_cast<const TutorialBattleSceneData::AttackTutorial *>(m_tutorialBattleSceneData->m_tutorialData[0].get());
		if(data!=nullptr){
			if(m_aimedUnit==data->m_targetUnit){
				//指定したユニットへの攻撃を確認できたら、次のチュートリアルデータへ
				GoNextTutorial();
				return true;//指定ユニットへの攻撃を指示した時のみ遷移処理を行う
			} else{
				//指定と違うユニットを狙っている場合は、攻撃ができないようにする。returnに到達させてはいけない。
				PlaySoundMem(GeneralPurposeResourceManager::cancelSound,DX_PLAYTYPE_BACK,TRUE);//失敗音を鳴らす
			}
		}
		return false;//攻撃ができない時は、遷移処理を行わない
	} else{
		//移動以外の動作がされている場合は、遷移処理をする(マップを調べるなど)
		return true;
	}
}

bool TutorialPlayerMoveScene::TutorialWaitProcess(int retIntPal){
	if(retIntPal==0){
		//待機については特に取得しないといけないデータはないのでキャストしない
		//次のチュートリアルデータへ
		GoNextTutorial();
		return true;//遷移処理を行う
	} else{
		//移動以外の動作がされている場合は、遷移処理をする(マップを調べるなど)
		return true;
	}
}

bool TutorialPlayerMoveScene::TutorialExplainProcess(){
	std::shared_ptr<TutorialBattleSceneData::ExplainTutorial> data=std::dynamic_pointer_cast<TutorialBattleSceneData::ExplainTutorial>(m_tutorialBattleSceneData->m_tutorialData[0]);
	if(data.get()!=nullptr){
		//説明チュートリアルの内部パラメータの更新
		//data->m_pos.Update();
		data->m_rate.Update();
		//説明チュートリアルの制御は、retIntPalを用いず、入力とチュートリアルデータのみを用いる
		if(!data->m_secondMoveFlag){
			//チュートリアル画面が入ってきてから真ん中で止まるまで
//			if(data->m_pos.GetEndFlag()
			if(data->m_rate.GetEndFlag()
				&& (mouse_get(MOUSE_INPUT_LEFT)==1
					|| keyboard_get(KEY_INPUT_Z)==1
					)
				)
			{
				//真ん中で停止した時にボタンを押していた場合、画面外に説明絵を移動させる
				data->SetSecondMoveFlag();
			}
		} else{
			//真ん中から外に出ていくまで
//			if(data->m_pos.GetEndFlag()){
			if(data->m_rate.GetEndFlag()){
				//外まで出て行ったら次のチュートリアルへ
				GoNextTutorial();
			}
		}
	}
	return false;
}

int TutorialPlayerMoveScene::thisCalculate(){
	//デバッグコマンドの入力
	if(keyboard_get(KEY_INPUT_LSHIFT)==60){
		m_battleSceneData->m_drawObjectShapeFlag=!m_battleSceneData->m_drawObjectShapeFlag;
	}

	//アニメーション進行
	m_animeFlame++;

	//キー入力受付
	const Vector2D mousePos=GetMousePointVector2D();

	std::pair<bool,int> retPal;
	for(const std::function<std::pair<bool,int>(PlayerMoveScene&)> &func:inCalculateProcessFunction){
		retPal=func(*this);
		if(retPal.first){
			//遷移の前に、チュートリアル進行情報を更新する
			bool transitionFlag=false;//遷移処理するかどうか
			//m_tutorialBattleSceneData->m_tutorialData[0]->m_kindを見れば、どの種類のチュートリアルをしているかわかる
			//retPal.secondを見れば、何の行動をしたかを見る事ができる
			if(!m_tutorialBattleSceneData->m_tutorialData.empty()){
				switch(m_tutorialBattleSceneData->m_tutorialData[0]->m_kind){
				case(TutorialBattleSceneData::TutorialBase::TutorialKind::e_move):
					transitionFlag=TutorialMoveProcess(retPal.second);
					break;
				case(TutorialBattleSceneData::TutorialBase::TutorialKind::e_normalAttack):
					transitionFlag=TutorialAttackProcess(retPal.second);
					break;
				case(TutorialBattleSceneData::TutorialBase::TutorialKind::e_wait):
					transitionFlag=TutorialWaitProcess(retPal.second);
					break;
				case(TutorialBattleSceneData::TutorialBase::TutorialKind::e_explain):
					transitionFlag=TutorialExplainProcess();
					break;
				}
			} else{
				//チュートリアル項目がない場合は、通常通り移動をした時以外は常に遷移するようにする
				if(retPal.second!=SceneKind::e_move){
					transitionFlag=true;
				}
			}
			//遷移処理を行う場合はretPal.secondを返すようにする
			if(transitionFlag){
				return retPal.second;
			}
		}
	}

	//次フレームに、本フレームにおけるマウスの位置が分かるようにする
	m_mousePosJustBefore=mousePos;

	return SceneKind::e_move;

}

void TutorialPlayerMoveScene::thisDraw()const{
	PlayerMoveScene::thisDraw();


	//デバッグ表示
	if(!m_tutorialBattleSceneData->m_tutorialData.empty() && m_tutorialBattleSceneData->m_tutorialData[0]->m_kind==TutorialBattleSceneData::TutorialBase::TutorialKind::e_move){
		const TutorialBattleSceneData::MoveTutorial *data=dynamic_cast<const TutorialBattleSceneData::MoveTutorial *>(m_tutorialBattleSceneData->m_tutorialData[0].get());
		if(data!=nullptr){
			//移動先の表示
			int mode,pal;
			GetDrawBlendMode(&mode,&pal);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,(int)(255*(std::sin((m_animeFlame%60)*M_PI/60))));
			data->m_moveTutorialArea->Draw(Vector2D(),data->m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())?GetColor(240,0,255):GetColor(128,0,64),TRUE,2.0f);
			SetDrawBlendMode(mode,pal);
		}
	}
	if(!m_tutorialBattleSceneData->m_tutorialData.empty() && m_tutorialBattleSceneData->m_tutorialData[0]->m_kind==TutorialBattleSceneData::TutorialBase::TutorialKind::e_normalAttack){
		const TutorialBattleSceneData::AttackTutorial *data=dynamic_cast<const TutorialBattleSceneData::AttackTutorial *>(m_tutorialBattleSceneData->m_tutorialData[0].get());
		if(data!=nullptr){
			//狙うユニットに矢印
			const int x=(int)(data->m_targetUnit->getPos().x+std::cosf((m_animeFlame%120)*M_PI/60)*5.0f)+30,y=(int)(data->m_targetUnit->getPos().y+std::sinf((m_animeFlame%120)*M_PI/60)*5.0f)-30;
			const unsigned int color=GetColor(128,0,255);
			DrawTriangle(x,y,x+5,y-30,x+30,y-5,color,TRUE);
			DrawTriangle(x+5,y-10,x+10,y-5,x+35,y-40,color,TRUE);
			DrawTriangle(x+40,y-35,x+10,y-5,x+35,y-40,color,TRUE);
		}
	}
	if(!m_tutorialBattleSceneData->m_tutorialData.empty() && m_tutorialBattleSceneData->m_tutorialData[0]->m_kind==TutorialBattleSceneData::TutorialBase::TutorialKind::e_wait){
		//待機ボタンに矢印
		int x,y,dx,dy;
		m_waitButton.GetButtonInfo(&x,&y,&dx,&dy);
		x=x+dx-5+(int)(std::cosf((m_animeFlame%120)*M_PI/60)*5.0f),y=y+5+(int)(std::sinf((m_animeFlame%120)*M_PI/60)*5.0f);
		const unsigned int color=GetColor(196,128,255);
		DrawTriangle(x,y,x+5,y-30,x+30,y-5,color,TRUE);
		DrawTriangle(x+5,y-10,x+10,y-5,x+35,y-40,color,TRUE);
		DrawTriangle(x+40,y-35,x+10,y-5,x+35,y-40,color,TRUE);
	}
	//待機ができないことの表示
	if(!m_waitableOnlyChangeInherit){
		int x,y,dx,dy;
		m_waitButton.GetButtonInfo(&x,&y,&dx,&dy);
		const int centerX=x+dx/2,centerY=y+dy/2;
		const unsigned int color=GetColor(255,128,128);
		DrawTriangle(centerX-60,centerY-40,centerX-40,centerY-60,centerX+60,centerY+40,color,TRUE);
		DrawTriangle(centerX-60,centerY-40,centerX+40,centerY+60,centerX+60,centerY+40,color,TRUE);
		DrawTriangle(centerX-60,centerY+40,centerX-40,centerY+60,centerX+60,centerY-40,color,TRUE);
		DrawTriangle(centerX-60,centerY+40,centerX+40,centerY-60,centerX+60,centerY-40,color,TRUE);
	}

	//説明文の描画(一番上に描画)
	if(!m_tutorialBattleSceneData->m_tutorialData.empty()){
		if(m_tutorialBattleSceneData->m_tutorialData[0]->m_kind==TutorialBattleSceneData::TutorialBase::TutorialKind::e_explain){
			//説明チュートリアルの場合は、半透明黒を画面全体に描画
			int mode,pal;
			GetDrawBlendMode(&mode,&pal);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
			DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
			SetDrawBlendMode(mode,pal);
		}
		m_tutorialBattleSceneData->m_tutorialData[0]->DrawSupplement(m_tutorialBattleSceneData->m_tutorialFont);
	}

}
