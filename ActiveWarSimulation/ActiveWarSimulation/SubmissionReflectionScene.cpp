#include"DxLib.h"
#include"input.h"
#include"SubmissionReflectionScene.h"
#include"StageClearScene.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"
#include"LogElement.h"

namespace {
	const int lineWidth=5;
	const unsigned int merginColor=GetColor(128,128,255);
	const float minimapRate=0.5f;
	const int minimapWidth=(int)(CommonConstParameter::mapSizeX*minimapRate);
	const int minimapHeight=(int)(CommonConstParameter::mapSizeY*minimapRate);
}

//---------------SubmissionReflectionScene-----------------
SubmissionReflectionScene::SubmissionReflectionScene(const std::shared_ptr<BattleSceneData> &battleSceneData,const std::shared_ptr<BattleSceneElement> &clearScene)
	:BattleSceneElement(SceneKind::e_submissionReflection)
	,m_battleSceneData(battleSceneData)
	,m_clearScene(clearScene)
{}

SubmissionReflectionScene::~SubmissionReflectionScene(){}

void SubmissionReflectionScene::DrawResizedMap(int x,int y,const std::vector<Unit> &unitList)const{
	//マージンの描画
	DrawBox(x-lineWidth,y-lineWidth,x+minimapWidth+lineWidth,y+minimapHeight+lineWidth,merginColor,TRUE);
	const Vector2D startPos((float)x,(float)y);
	//BattleSceneDataの描画関数は拡大縮小描画に対応していないので、独自に実装する
	//背景描画
	DrawExtendGraphExRateAssign(startPos.x,startPos.y,minimapRate,m_battleSceneData->m_mapPic,TRUE);
	//ユニットの描画
	for(const Unit unit:unitList){
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&unit)
			&& unit.GetFix()!=Shape::Fix::e_ignore)
		{
			//ウインドウに入っていない物は描画しない
			//退却したユニット(m_fixがe_ignore)は描画しない
			unit.DrawUnit(unit.getPos(),startPos,minimapRate,0,false,true,false);
		}
	}
	//ユニットのHPゲージの描画
	for(const Unit unit:unitList){
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&unit)
			&& unit.GetFix()!=Shape::Fix::e_ignore)
		{
			//ウインドウに入っていない物は描画しない
			//退却したユニット(m_fixがe_ignore)は描画しない
			unit.DrawHPGage(unit.getPos(),startPos,minimapRate);
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
	const WholeReflectionInfo reflectionInfo=m_battleSceneData->m_scoreObserver->GetSubmission().GetReflectionInfo();
	
	if(reflectionInfo.m_badLog.second && reflectionInfo.m_goodLog.second){
		std::vector<Unit> goodUnitList,badUnitList;
		for(const LogElement::UnitLogData &logData:reflectionInfo.m_goodLog.second->m_unitDataList){
			//良いとされる盤面のユニットの作成
			Unit u=*logData.punit;
			u.Warp(logData.pos);
			u.AddHP(logData.hp-u.GetBattleStatus().HP);
			u.SetOP(logData.op);
			//格納
			goodUnitList.push_back(u);
		}
		for(const LogElement::UnitLogData &logData:reflectionInfo.m_badLog.second->m_unitDataList){
			//悪いとされる盤面のユニットの作成
			Unit u=*logData.punit;
			u.Warp(logData.pos);
			u.AddHP(logData.hp-u.GetBattleStatus().HP);
			u.SetOP(logData.op);
			//格納
			badUnitList.push_back(u);
		}
		DrawResizedMap(40,40,goodUnitList);
		DrawResizedMap(900,500,badUnitList);
	}
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
