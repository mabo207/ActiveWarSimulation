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
	//�f�o�b�O�R�}���h�̓���
	if(keyboard_get(KEY_INPUT_LSHIFT)==60){
		m_battleSceneData->m_drawObjectShapeFlag=!m_battleSceneData->m_drawObjectShapeFlag;
	}

	//�A�j���[�V�����i�s
	m_animeFlame++;

	//�L�[���͎�t
	const Vector2D mousePos=GetMousePointVector2D();

	std::pair<bool,int> retPal;
	for(const std::function<std::pair<bool,int>(PlayerMoveScene&)> &func:inCalculateProcessFunction){
		retPal=func(*this);
		if(retPal.first && retPal.second!=SceneKind::e_move){
			return retPal.second;
		}
	}

	//���t���[���ɁA�{�t���[���ɂ�����}�E�X�̈ʒu��������悤�ɂ���
	m_mousePosJustBefore=mousePos;

	//�`���[�g���A���i�s���̍X�V
	if(m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())){
		//�ڕW�ړ��n�_�ւ̓��B����
		//�ڕW�ړ��G���A�Ƀ��j�b�g�̒��S�_����������
		m_moveTutorialFlag=false;
	}

	return SceneKind::e_move;

}

void TutorialPlayerMoveScene::thisDraw()const{
	PlayerMoveScene::thisDraw();

	//�f�o�b�O�\��
	if(true){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,(int)(255*(std::sin((m_animeFlame%60)*M_PI/60))));
		m_moveTutorialArea->Draw(Vector2D(),m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())?GetColor(240,0,255):GetColor(128,0,64),TRUE,2.0f);
		SetDrawBlendMode(mode,pal);
	}
}
