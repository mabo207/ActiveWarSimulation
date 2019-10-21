#include"DxLib.h"
#include"input.h"
#include"SubmissionReflectionScene.h"
#include"StageClearScene.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"

//---------------SubmissionReflectionScene-----------------
SubmissionReflectionScene::SubmissionReflectionScene(const std::shared_ptr<BattleSceneData> &battleSceneData,const std::shared_ptr<BattleSceneElement> &clearScene)
	:BattleSceneElement(SceneKind::e_submissionReflection)
	,m_battleSceneData(battleSceneData)
	,m_clearScene(clearScene)
{}

SubmissionReflectionScene::~SubmissionReflectionScene(){}

void SubmissionReflectionScene::DrawResizedMap(float exRate,Vector2D startPos)const{
	//BattleSceneDataの描画関数は拡大縮小描画に対応していないので、独自に実装する
	//背景描画
	DrawExtendGraphExRateAssign(startPos.x,startPos.y,exRate,m_battleSceneData->m_mapPic,TRUE);
	//ユニットの描画
	for(const Unit * const pUnit:m_battleSceneData->m_unitList){
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(pUnit)
			&& pUnit->GetFix()!=Shape::Fix::e_ignore)
		{
			//ウインドウに入っていない物は描画しない
			//退却したユニット(m_fixがe_ignore)は描画しない
			pUnit->DrawUnit(pUnit->getPos(),startPos,exRate,0,false,true,false);
		}
	}
	//ユニットのHPゲージの描画
	for(const Unit * const pUnit:m_battleSceneData->m_unitList){
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(pUnit)
			&& pUnit->GetFix()!=Shape::Fix::e_ignore)
		{
			//ウインドウに入っていない物は描画しない
			//退却したユニット(m_fixがe_ignore)は描画しない
			pUnit->DrawHPGage(pUnit->getPos(),startPos,exRate);
		}
	}
}

int SubmissionReflectionScene::thisCalculate(){
	if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
		return SceneKind::e_clear;
	}

	return SceneKind::e_submissionReflection;
}

void SubmissionReflectionScene::thisDraw()const{
	const int lineWidth=5;
	const int x1=10,y1=40,x2=900,y2=500;
	DrawBox(x1-lineWidth,y1-lineWidth,x1+CommonConstParameter::mapSizeX/2+lineWidth,y1+CommonConstParameter::mapSizeY/2+lineWidth,GetColor(128,128,255),TRUE);
	DrawResizedMap(0.5,Vector2D((float)x1,(float)y1));
	DrawBox(x2-lineWidth,y2-lineWidth,x2+CommonConstParameter::mapSizeX/2+lineWidth,y2+CommonConstParameter::mapSizeY/2+lineWidth,GetColor(128,128,255),TRUE);
	DrawResizedMap(0.5,Vector2D((float)x2,(float)y2));
}

int SubmissionReflectionScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_clear):
		m_nextScene=m_clearScene;
		m_clearScene.reset();//m_nextSceneにコピーしたので、所有権を放棄
		break;
	}
	return index;
}

void SubmissionReflectionScene::ReturnProcess(){
	//特に何もしない
}
