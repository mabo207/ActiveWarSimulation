#define _USE_MATH_DEFINES
#include"DxLib.h"
#include"TutorialPlayerMoveScene.h"
#include"Circle.h"
#include"GeneralPurposeResourceManager.h"
#include<cmath>

//-------------------TutorialPlayerMoveScene---------------
TutorialPlayerMoveScene::TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:PlayerMoveScene(battleSceneData),m_tutorialBattleSceneData(std::dynamic_pointer_cast<TutorialBattleSceneData>(battleSceneData))
	,m_moveTutorialArea(new Circle(Vector2D(395.0f,425.0f),45.0f,Shape::Fix::e_ignore))
	,m_animeFlame(0)
	,m_targetUnit(m_battleSceneData->m_unitList[1])
{
	//初期状態では移動しかできないようにする
	m_attackableOnlyChangeInherit=false;
	m_waitableOnlyChangeInherit=false;
	//移動チュートリアルから
	m_moveTutorialFlag=true;
	m_attackTutorialFlag=false;
}

TutorialPlayerMoveScene::~TutorialPlayerMoveScene(){}

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
			//retPal.secondを見れば、何の行動をしたかを見る事ができる
			if(m_moveTutorialFlag
				&& retPal.second==SceneKind::e_move
				&& m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())
				)
			{
				//移動系のチュートリアル進行情報の更新
				//移動チュートリアル中の目標移動地点への到達判定
				//目標移動エリアにユニットの中心点が入ったら
				m_moveTutorialFlag=false;
				m_moveableOnlyChangeInherit=false;//移動できなくする
				m_attackableOnlyChangeInherit=true;//攻撃できるようにする
				m_waitableOnlyChangeInherit=false;//待機できなくする
				m_attackTutorialFlag=true;//攻撃チュートリアル開始
			} else if(m_attackTutorialFlag
				&& retPal.second==SceneKind::e_attackNormal
				)
			{
				//通常攻撃チュートリアル進行情報の更新
				//攻撃対象ユニットに狙いをつけている時に攻撃したかどうかを判定する必要がある
				if(m_aimedUnit==m_targetUnit){
					//ちゃんと正しく狙っている場合は、行動成功
					m_attackTutorialFlag=false;
				} else{
					//指定と違うユニットを狙っている場合は、攻撃ができないようにする。returnに到達させてはいけない。
					PlaySoundMem(GeneralPurposeResourceManager::cancelSound,DX_PLAYTYPE_BACK,TRUE);//失敗音を鳴らす
					continue;
				}
			}

			if(retPal.second!=SceneKind::e_move){
				//移動系処理の場合以外は遷移処理をする
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
	if(m_moveTutorialFlag){
		//移動先の表示
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,(int)(255*(std::sin((m_animeFlame%60)*M_PI/60))));
		m_moveTutorialArea->Draw(Vector2D(),m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())?GetColor(240,0,255):GetColor(128,0,64),TRUE,2.0f);
		SetDrawBlendMode(mode,pal);
	}
	if(m_attackTutorialFlag){
		//狙うユニットの表示
		const int x=(int)(m_targetUnit->getPos().x+std::cosf((m_animeFlame%120)*M_PI/60)*5.0f)+30,y=(int)(m_targetUnit->getPos().y+std::sinf((m_animeFlame%120)*M_PI/60)*5.0f)-30;
		const unsigned int color=GetColor(128,0,255);
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
}
