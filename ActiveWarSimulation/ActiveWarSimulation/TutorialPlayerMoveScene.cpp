#define _USE_MATH_DEFINES
#include"DxLib.h"
#include"TutorialPlayerMoveScene.h"
#include"Circle.h"
#include"GeneralPurposeResourceManager.h"
#include<cmath>

//-------------------TutorialPlayerMoveScene---------------
TutorialPlayerMoveScene::TutorialPlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:PlayerMoveScene(battleSceneData)
	,m_tutorialBattleSceneData(std::dynamic_pointer_cast<TutorialBattleSceneData>(battleSceneData))
	,m_animeFlame(0)
{
	//�擪�̃`���[�g���A�����ڂɏ]����bool�l���X�V
	UpdateFlagOnlyInherit();
}

TutorialPlayerMoveScene::~TutorialPlayerMoveScene(){}

void TutorialPlayerMoveScene::UpdateFlagOnlyInherit(){
	if(!m_tutorialBattleSceneData->m_tutorialData.empty()){
		//�`���[�g���A�����ڂ��c���Ă���ꍇ�Am_tutorialBattleSceneData��p���ďo���铮��𐧌�
		//�܂��S�Ăł��Ȃ�������
		m_moveableOnlyChangeInherit=false;
		m_attackableOnlyChangeInherit=false;
		m_waitableOnlyChangeInherit=false;
		//���ɁA�o����s����ݒ肷��
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
		default:
			break;
		}
	} else{
		//�`���[�g���A�����ڂ��c���Ă��Ȃ��ꍇ�A���ł��ł���悤�ɂ���
		m_moveableOnlyChangeInherit=true;
		m_attackableOnlyChangeInherit=true;
		m_waitableOnlyChangeInherit=true;
	}
}

void TutorialPlayerMoveScene::GoNextTutorial(){
	//�擪�̃`���[�g���A���f�[�^����菜��
	m_tutorialBattleSceneData->m_tutorialData.erase(m_tutorialBattleSceneData->m_tutorialData.begin());
	//�t���O�̍X�V
	UpdateFlagOnlyInherit();
}

bool TutorialPlayerMoveScene::TutorialMoveProcess(int retIntPal){
	if(retIntPal==SceneKind::e_move){
		//�ړ����������Ƃ��m�F�������̂݁A�L���X�g����B
		const TutorialBattleSceneData::MoveTutorial *data=dynamic_cast<const TutorialBattleSceneData::MoveTutorial *>(m_tutorialBattleSceneData->m_tutorialData[0].get());
		if(data!=nullptr
			&& data->m_moveTutorialArea->VJudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())
			)
		{
			//�ړI�n�Ɉړ��ł����̂ŁA���̃`���[�g���A���f�[�^�֍X�V
			GoNextTutorial();
		}
	}
	return false;//�J�ڏ������s��Ȃ�
}

bool TutorialPlayerMoveScene::TutorialAttackProcess(int retIntPal){
	if(retIntPal==SceneKind::e_attackNormal){
		//�U�������s���悤�Ƃ������͂����m�������̂݁A�L���X�g����
		const TutorialBattleSceneData::AttackTutorial *data=dynamic_cast<const TutorialBattleSceneData::AttackTutorial *>(m_tutorialBattleSceneData->m_tutorialData[0].get());
		if(data!=nullptr){
			if(m_aimedUnit==data->m_targetUnit){
				//�w�肵�����j�b�g�ւ̍U�����m�F�ł�����A���̃`���[�g���A���f�[�^��
				GoNextTutorial();
				return true;//�w�胆�j�b�g�ւ̍U�����w���������̂ݑJ�ڏ������s��
			} else{
				//�w��ƈႤ���j�b�g��_���Ă���ꍇ�́A�U�����ł��Ȃ��悤�ɂ���Breturn�ɓ��B�����Ă͂����Ȃ��B
				PlaySoundMem(GeneralPurposeResourceManager::cancelSound,DX_PLAYTYPE_BACK,TRUE);//���s����炷
			}
		}
	}
	return false;
}

bool TutorialPlayerMoveScene::TutorialWaitProcess(int retIntPal){
	if(retIntPal==0){
		//�ҋ@�ɂ��Ă͓��Ɏ擾���Ȃ��Ƃ����Ȃ��f�[�^�͂Ȃ��̂ŃL���X�g���Ȃ�
		//���̃`���[�g���A���f�[�^��
		GoNextTutorial();
		return true;//�J�ڏ������s��
	}
	return false;
}

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
			bool transitionFlag=false;//�J�ڏ������邩�ǂ���
			//m_tutorialBattleSceneData->m_tutorialData[0]->m_kind������΁A�ǂ̎�ނ̃`���[�g���A�������Ă��邩�킩��
			//retPal.second������΁A���̍s���������������鎖���ł���
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
				default:
					break;
				}
			} else{
				//�`���[�g���A�����ڂ��Ȃ��ꍇ�́A�ʏ�ʂ�ړ����������ȊO�͏�ɑJ�ڂ���悤�ɂ���
				if(retPal.second!=SceneKind::e_move){
					transitionFlag=true;
				}
			}
			//�J�ڏ������s���ꍇ��retPal.second��Ԃ��悤�ɂ���
			if(transitionFlag){
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
	if(!m_tutorialBattleSceneData->m_tutorialData.empty() && m_tutorialBattleSceneData->m_tutorialData[0]->m_kind==TutorialBattleSceneData::TutorialBase::TutorialKind::e_move){
		const TutorialBattleSceneData::MoveTutorial *data=dynamic_cast<const TutorialBattleSceneData::MoveTutorial *>(m_tutorialBattleSceneData->m_tutorialData[0].get());
		if(data!=nullptr){
			//�ړ���̕\��
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
			//�_�����j�b�g�ɖ��
			const int x=(int)(data->m_targetUnit->getPos().x+std::cosf((m_animeFlame%120)*M_PI/60)*5.0f)+30,y=(int)(data->m_targetUnit->getPos().y+std::sinf((m_animeFlame%120)*M_PI/60)*5.0f)-30;
			const unsigned int color=GetColor(128,0,255);
			DrawTriangle(x,y,x+5,y-30,x+30,y-5,color,TRUE);
			DrawTriangle(x+5,y-10,x+10,y-5,x+35,y-40,color,TRUE);
			DrawTriangle(x+40,y-35,x+10,y-5,x+35,y-40,color,TRUE);
		}
	}
	if(!m_tutorialBattleSceneData->m_tutorialData.empty() && m_tutorialBattleSceneData->m_tutorialData[0]->m_kind==TutorialBattleSceneData::TutorialBase::TutorialKind::e_wait){
		//�ҋ@�{�^���ɖ��
		int x,y,dx,dy;
		m_waitButton.GetButtonInfo(&x,&y,&dx,&dy);
		x=x+dx-5+(int)(std::cosf((m_animeFlame%120)*M_PI/60)*5.0f),y=y+5+(int)(std::sinf((m_animeFlame%120)*M_PI/60)*5.0f);
		const unsigned int color=GetColor(196,128,255);
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