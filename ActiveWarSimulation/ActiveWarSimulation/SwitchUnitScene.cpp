#include"DxLib.h"
#include"SwitchUnitScene.h"
#include"PlayerMoveScene.h"
#include"ComputerMoveScene.h"
#include"StageClearScene.h"
#include"GraphicControl.h"
#include"CommonConstParameter.h"

#include"TutorialPlayerMoveScene.h"

//------------------SwitchUnitScene-------------------
SwitchUnitScene::SwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_switch)
	,m_battleSceneData(battleSceneData)
	,m_judgeEnd(JudgeEnd::e_notEnd)
	,m_rubricWordPosition(-50,-50)
	,m_turnFont(CreateFontToHandleEX("���C���I",80,8,DX_FONTTYPE_ANTIALIASING_EDGE_4X4,-1,3))
{
	//�i�s�f�[�^���X�V
	ReturnProcess();
	if(m_judgeEnd==JudgeEnd::e_notEnd){
		//�o�g���J�n����͂��Ȃ炸e_notEnd����e_notEnd_TurnChange�ɐ؂�ւ���
		//�������A�v���O�����̍\����ς��ă^�[���؂�ւ��̂��тɂ��̃R���X�g���N�^���Ă΂��悤�ɂȂ�Ɩ��ɂȂ��Ă��܂��̂ŁA�����𒼂������B
		m_judgeEnd=JudgeEnd::e_notEnd_TurnChange;
	}
}

SwitchUnitScene::~SwitchUnitScene(){
	DeleteFontToHandleEX(m_turnFont);
}

int SwitchUnitScene::CalculateTurn()const{
	return m_battleSceneData->CalculateTurn();
}

std::shared_ptr<BattleSceneElement> SwitchUnitScene::GetPlayerMoveScene()const{
	if(m_battleSceneData->m_playMode!=BattleSceneData::PlayMode::e_tutorial){
		//�ʏ�o�g���ł���ꍇ��PlayerMoveScene��
		return std::shared_ptr<BattleSceneElement>(new PlayerMoveScene(m_battleSceneData));
	} else{
		//�`���[�g���A���ł���ꍇ��TutorialPlayerMoveScene��
		return std::shared_ptr<BattleSceneElement>(new TutorialPlayerMoveScene(m_battleSceneData));
	}
}

bool SwitchUnitScene::JudgeTimeProcessed()const{
	return m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0f;
}

bool SwitchUnitScene::JudgeGoToMoveScene()const{
	//�T�u�~�b�V��������`�悷�邩�ǂ����Ō��܂�
	if(JudgeDrawRubric()){
		//�T�u�~�b�V��������`�悷�鎞�́A�|�b�v�A�b�v����ʊO�ɏo�Ă���
		return m_rubricPopupExiting & m_rubricReasonPosition.GetEndFlag();
	} else{
		//�T�u�~�b�V��������`�悵�Ȃ����́A���Ԍo�߂ŗǂ�
		return JudgeTimeProcessed();
	}
}

bool SwitchUnitScene::JudgeDrawRubric()const{
	//�T�u�~�b�V�����@�\�������Ă��āA�Ȃ����`�悷��]���̓��e���`��\���e�ł���(-1�]���Ȃǂ͕`�悵�Ȃ��Ȃǂ�����)��
	return m_battleSceneData->m_submissionRunFlag && m_battleSceneData->m_scoreObserver->GetSubmission().JudgeDrawRubric();
}

int SwitchUnitScene::thisCalculate(){
	//�^�C�}�[�X�V
	m_battleSceneData->m_fpsMesuring.Update();
	//���[�u���b�N�`��ʒu�̍X�V
	m_rubricWordPosition.Update();
	m_rubricReasonPosition.Update();
	
	//���s����ɂ���āA�J�ڏ����͕���
	switch(m_judgeEnd){
	case(JudgeEnd::e_notEnd):
	case(JudgeEnd::e_notEnd_TurnChange):
		//���������Ă��Ȃ�
		//��莞�Ԍo�����烆�j�b�g�ړ���
		if(JudgeGoToMoveScene()){
			//���j�b�g�ڍs������
			m_battleSceneData->m_fpsMesuring.RecordTime();//�^�C�}�[������
			return SceneKind::e_move;
		} else if(!m_rubricPopupExiting && JudgeTimeProcessed() && (mouse_get(MOUSE_INPUT_LEFT)==1 || keyboard_get(KEY_INPUT_Z)==1)){
			//�ڍs���Ԃ��o���Ĉȍ~�Ɏ��ɐi�ޓ��͂�������A�|�b�v�A�b�v��V���Ȉʒu��
			//�T�u�~�b�V�����̃��[�u���b�N�]���̕\�����Ȃ����́AJudgeGoToMoveScene()��JudgeTimeProcessed()�ƈ�v����̂ŁA�����ɂ͗��Ȃ��B
			m_rubricPopupExiting=true;
			m_rubricReasonPosition.SetTarget(m_rubricReasonPosition.GetX(),-SelfDecideSubmission::s_reasonHeight,true);
		}
		break;
	case(JudgeEnd::e_playerWin):
		//�v���C���[������
		return SceneKind::e_clear;
		break;
	case(JudgeEnd::e_playerLose):
		//�v���C���[���s�k
		return SceneKind::e_clear;
		break;
	}
	
	return SceneKind::e_switch;
}

void SwitchUnitScene::thisDraw()const{
	//�}�b�v�̕\��
	m_battleSceneData->DrawField();
	//���j�b�g�̕\��
	m_battleSceneData->DrawUnit(false,{});
	//HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();
	//�I�[�_�[�̕\���͕`�悵�Ȃ�

	//�^�[���؂�ւ��\��
	if(m_judgeEnd==JudgeEnd::e_notEnd_TurnChange){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		DrawBox(1020,750,1920,850,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(mode,pal);
		DrawStringToHandle(1030,760,("BATTLE PHASE "+to_string_0d(CalculateTurn(),2)).c_str(),GetColor(255,255,255),m_turnFont,GetColor(0,0,0));
	}

	//�T�u�~�b�V�����]���\��
	if(JudgeDrawRubric()){
		m_battleSceneData->m_scoreObserver->GetSubmission().DrawRubric(m_rubricWordPosition.GetX(),m_rubricWordPosition.GetY());
		m_battleSceneData->m_scoreObserver->GetSubmission().DrawReason(m_rubricReasonPosition.GetX(),m_rubricReasonPosition.GetY());
	}
}

int SwitchUnitScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_move):
		if(m_battleSceneData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
			//�v���C���[�L�����Ȃ玩����N���X��
			m_nextScene=GetPlayerMoveScene();
		} else{
			//����ȊO�Ȃ�AI����N���X��
			m_nextScene=std::shared_ptr<BattleSceneElement>(new ComputerMoveScene(m_battleSceneData));
		}
		return index;
	case(SceneKind::e_clear):
		m_battleSceneData->m_scoreObserver->FinishUpdate(m_battleSceneData.get());
		if(m_judgeEnd==JudgeEnd::e_playerWin){
			//�v���C���[�̏���
			m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,true));
		} else if(m_judgeEnd==JudgeEnd::e_playerLose){
			//�v���C���[�̔s�k
			m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,false));
		}
		return index;
	default:
		return index;
	}
}

void SwitchUnitScene::ReturnProcess(){
	m_battleSceneData->m_fpsMesuring.RecordTime();//�^�C�}�[������
	//���j�b�g�؂�ւ��O�Ƀ��[�u���b�N�]���֘A�̐ݒ�
	if(m_battleSceneData->m_operateUnit!=nullptr){
		//�����ʒu�̐ݒ�
		m_rubricWordPosition=PositionControl((int)m_battleSceneData->m_operateUnit->getPos().x,(int)m_battleSceneData->m_operateUnit->getPos().y-10,10,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0);
	}
	m_rubricWordPosition.SetTarget(m_rubricWordPosition.GetX(),m_rubricWordPosition.GetY()-50,true);//�����ʒu���50px��Ɉړ�
	m_rubricReasonPosition=PositionControl(CommonConstParameter::gameResolutionX-SelfDecideSubmission::s_reasonWidth,-SelfDecideSubmission::s_reasonHeight,20,Easing::TYPE_IN,Easing::FUNCTION_LINER,0.0);
	m_rubricReasonPosition.SetTarget(m_rubricReasonPosition.GetX(),0,true);
	m_rubricPopupExiting=false;
	//���s����
	size_t playerUnitNum=0,enemyUnitNum=0;
	for(size_t i=0,size=m_battleSceneData->m_unitList.size();i<size;i++){
		switch(m_battleSceneData->m_unitList[i]->GetBattleStatus().team){
		case(Unit::Team::e_player):
			playerUnitNum++;
			break;
		case(Unit::Team::e_enemy):
			enemyUnitNum++;
			break;
		default:
			break;
		}
	}
	if(playerUnitNum==0){
		//�v���C���[�L������0�Ȃ畉��
		m_judgeEnd=JudgeEnd::e_playerLose;
	} else if(enemyUnitNum==0){
		//�G�L������0�Ȃ珟��
		m_judgeEnd=JudgeEnd::e_playerWin;
	} else{
		//�ǂ�ł��Ȃ��Ȃ�퓬���s
		const int beforeTurn=CalculateTurn();
		m_battleSceneData->FinishUnitOperation();//���j�b�g�̐؂�ւ�����(�퓬���s�ȊO�̎��͂��Ȃ��A�N���A�^�[�����̌v�Z���]���ɉ��Z���ꂩ�˂Ȃ�����)
		if(CalculateTurn()!=beforeTurn){
			//�؂�ւ������̑O��Ń^�[�������ς���Ă�����A�^�[���؂�ւ���ʂ�\������悤�ɂ���
			m_judgeEnd=JudgeEnd::e_notEnd_TurnChange;
		} else{
			//�؂�ւ������̑O��Ń^�[�������ς���Ă�����A�^�[���؂�ւ��\���͂��Ȃ�
			m_judgeEnd=JudgeEnd::e_notEnd;
		}
	}
}
