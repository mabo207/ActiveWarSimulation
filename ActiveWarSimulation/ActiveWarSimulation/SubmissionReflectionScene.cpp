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
	//BattleSceneDataÇÃï`âÊä÷êîÇÕägëÂèkè¨ï`âÊÇ…ëŒâûÇµÇƒÇ¢Ç»Ç¢ÇÃÇ≈ÅAì∆é©Ç…é¿ëïÇ∑ÇÈ
	//îwåiï`âÊ
	DrawExtendGraphExRateAssign(startPos.x,startPos.y,exRate,m_battleSceneData->m_mapPic,TRUE);
	//ÉÜÉjÉbÉgÇÃï`âÊ
	for(const Unit * const pUnit:m_battleSceneData->m_unitList){
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(pUnit)
			&& pUnit->GetFix()!=Shape::Fix::e_ignore)
		{
			//ÉEÉCÉìÉhÉEÇ…ì¸Ç¡ÇƒÇ¢Ç»Ç¢ï®ÇÕï`âÊÇµÇ»Ç¢
			//ëﬁãpÇµÇΩÉÜÉjÉbÉg(m_fixÇ™e_ignore)ÇÕï`âÊÇµÇ»Ç¢
			pUnit->DrawUnit(pUnit->getPos(),startPos,exRate,0,false,true,false);
		}
	}
	//ÉÜÉjÉbÉgÇÃHPÉQÅ[ÉWÇÃï`âÊ
	for(const Unit * const pUnit:m_battleSceneData->m_unitList){
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(pUnit)
			&& pUnit->GetFix()!=Shape::Fix::e_ignore)
		{
			//ÉEÉCÉìÉhÉEÇ…ì¸Ç¡ÇƒÇ¢Ç»Ç¢ï®ÇÕï`âÊÇµÇ»Ç¢
			//ëﬁãpÇµÇΩÉÜÉjÉbÉg(m_fixÇ™e_ignore)ÇÕï`âÊÇµÇ»Ç¢
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
		m_clearScene.reset();//m_nextSceneÇ…ÉRÉsÅ[ÇµÇΩÇÃÇ≈ÅAèäóLå†Çï˙ä¸
		break;
	}
	return index;
}

void SubmissionReflectionScene::ReturnProcess(){
	//ì¡Ç…âΩÇ‡ÇµÇ»Ç¢
}
