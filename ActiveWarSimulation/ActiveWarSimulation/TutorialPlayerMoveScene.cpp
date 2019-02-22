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
	//������Ԃł͈ړ������ł��Ȃ��悤�ɂ���
	m_attackableOnlyChangeInherit=false;
	m_waitableOnlyChangeInherit=false;
	//�ړ��`���[�g���A������
	m_moveTutorialFlag=true;
	m_attackTutorialFlag=false;
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
		if(retPal.first){
			//�J�ڂ̑O�ɁA�`���[�g���A���i�s�����X�V����
			//retPal.second������΁A���̍s���������������鎖���ł���
			if(m_moveTutorialFlag
				&& retPal.second==SceneKind::e_move
				&& m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())
				)
			{
				//�ړ��n�̃`���[�g���A���i�s���̍X�V
				//�ړ��`���[�g���A�����̖ڕW�ړ��n�_�ւ̓��B����
				//�ڕW�ړ��G���A�Ƀ��j�b�g�̒��S�_����������
				m_moveTutorialFlag=false;
				m_moveableOnlyChangeInherit=false;//�ړ��ł��Ȃ�����
				m_attackableOnlyChangeInherit=true;//�U���ł���悤�ɂ���
				m_waitableOnlyChangeInherit=false;//�ҋ@�ł��Ȃ�����
				m_attackTutorialFlag=true;//�U���`���[�g���A���J�n
			} else if(m_attackTutorialFlag
				&& retPal.second==SceneKind::e_attackNormal
				)
			{
				//�ʏ�U���`���[�g���A���i�s���̍X�V
				//�U���Ώۃ��j�b�g�ɑ_�������Ă��鎞�ɍU���������ǂ����𔻒肷��K�v������
				if(m_aimedUnit==m_targetUnit){
					//�����Ɛ������_���Ă���ꍇ�́A�s������
					m_attackTutorialFlag=false;
				} else{
					//�w��ƈႤ���j�b�g��_���Ă���ꍇ�́A�U�����ł��Ȃ��悤�ɂ���Breturn�ɓ��B�����Ă͂����Ȃ��B
					PlaySoundMem(GeneralPurposeResourceManager::cancelSound,DX_PLAYTYPE_BACK,TRUE);//���s����炷
					continue;
				}
			}

			if(retPal.second!=SceneKind::e_move){
				//�ړ��n�����̏ꍇ�ȊO�͑J�ڏ���������
				return retPal.second;
			}
		}
	}

	//���t���[���ɁA�{�t���[���ɂ�����}�E�X�̈ʒu��������悤�ɂ���
	m_mousePosJustBefore=mousePos;

	return SceneKind::e_move;

}

void TutorialPlayerMoveScene::thisDraw()const{
	PlayerMoveScene::thisDraw();

	//�f�o�b�O�\��
	if(m_moveTutorialFlag){
		//�ړ���̕\��
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,(int)(255*(std::sin((m_animeFlame%60)*M_PI/60))));
		m_moveTutorialArea->Draw(Vector2D(),m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())?GetColor(240,0,255):GetColor(128,0,64),TRUE,2.0f);
		SetDrawBlendMode(mode,pal);
	}
	if(m_attackTutorialFlag){
		//�_�����j�b�g�̕\��
		const int x=(int)(m_targetUnit->getPos().x+std::cosf((m_animeFlame%120)*M_PI/60)*5.0f)+30,y=(int)(m_targetUnit->getPos().y+std::sinf((m_animeFlame%120)*M_PI/60)*5.0f)-30;
		const unsigned int color=GetColor(128,0,255);
		DrawTriangle(x,y,x+5,y-30,x+30,y-5,color,TRUE);
		DrawTriangle(x+5,y-10,x+10,y-5,x+35,y-40,color,TRUE);
		DrawTriangle(x+40,y-35,x+10,y-5,x+35,y-40,color,TRUE);
	}
	//�ҋ@���ł��Ȃ����Ƃ̕\��
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
