#include"DxLib.h"
#include"input.h"
#include"SubmissionReflectionScene.h"
#include"StageClearScene.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"

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

void SubmissionReflectionScene::DrawResizedMap(int x,int y)const{
	//�}�[�W���̕`��
	DrawBox(x-lineWidth,y-lineWidth,x+minimapWidth+lineWidth,y+minimapHeight+lineWidth,merginColor,TRUE);
	const Vector2D startPos((float)x,(float)y);
	//BattleSceneData�̕`��֐��͊g��k���`��ɑΉ����Ă��Ȃ��̂ŁA�Ǝ��Ɏ�������
	//�w�i�`��
	DrawExtendGraphExRateAssign(startPos.x,startPos.y,minimapRate,m_battleSceneData->m_mapPic,TRUE);
	//���j�b�g�̕`��
	for(const Unit * const pUnit:m_battleSceneData->m_unitList){
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(pUnit)
			&& pUnit->GetFix()!=Shape::Fix::e_ignore)
		{
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			//�ދp�������j�b�g(m_fix��e_ignore)�͕`�悵�Ȃ�
			pUnit->DrawUnit(pUnit->getPos(),startPos,minimapRate,0,false,true,false);
		}
	}
	//���j�b�g��HP�Q�[�W�̕`��
	for(const Unit * const pUnit:m_battleSceneData->m_unitList){
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(pUnit)
			&& pUnit->GetFix()!=Shape::Fix::e_ignore)
		{
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			//�ދp�������j�b�g(m_fix��e_ignore)�͕`�悵�Ȃ�
			pUnit->DrawHPGage(pUnit->getPos(),startPos,minimapRate);
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
	DrawResizedMap(40,40);
	DrawResizedMap(900,500);
}

int SubmissionReflectionScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_clear):
		m_nextScene=m_clearScene;
		m_clearScene.reset();//m_nextScene�ɃR�s�[�����̂ŁA���L�������
		break;
	}
	return index;
}

void SubmissionReflectionScene::ReturnProcess(){
	//���ɉ������Ȃ�
}
