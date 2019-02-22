#define _USE_MATH_DEFINES
#include"DxLib.h"
#include"TutorialPlayerMoveScene.h"
#include"Circle.h"
#include<cmath>

//-------------------TutorialPlayerMoveScene---------------
TutorialPlayerMoveScene::TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:PlayerMoveScene(battleSceneData),m_tutorialBattleSceneData(std::dynamic_pointer_cast<TutorialBattleSceneData>(battleSceneData))
	,m_moveTutorialArea(new Circle(Vector2D(395.0f,425.0f),45.0f,Shape::Fix::e_ignore))
	,m_animeFlame(0)
{
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
		if(retPal.first && retPal.second!=SceneKind::e_move){
			return retPal.second;
		}
	}

	//次フレームに、本フレームにおけるマウスの位置が分かるようにする
	m_mousePosJustBefore=mousePos;

	//チュートリアル進行情報の更新
	if(m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())){
		//目標移動地点への到達判定
		//目標移動エリアにユニットの中心点が入ったら
		m_moveTutorialFlag=false;
	}

	return SceneKind::e_move;

}

void TutorialPlayerMoveScene::thisDraw()const{
	PlayerMoveScene::thisDraw();

	//デバッグ表示
	if(true){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,(int)(255*(std::sin((m_animeFlame%60)*M_PI/60))));
		m_moveTutorialArea->Draw(Vector2D(),m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())?GetColor(240,0,255):GetColor(128,0,64),TRUE,2.0f);
		SetDrawBlendMode(mode,pal);
	}
}
