#include"DxLib.h"
#include"PlayerMoveScene.h"
#include"GraphicControl.h"
#include"GeneralPurposeResource.h"
#include<iostream>
#include"FilePath.h"
#include"CommonConstParameter.h"

namespace{
	int GetSubmissionX(){
		return CommonConstParameter::gameResolutionX-SelfDecideSubmission::s_submissionWidth;
	}
}

//----------------------PlayerMoveScene------------------------
const std::array<std::function<std::pair<bool,int>(PlayerMoveScene&)>,11> PlayerMoveScene::inCalculateProcessFunction={
	&PlayerMoveScene::AttackProcess
	,&PlayerMoveScene::SkillProcess
	,&PlayerMoveScene::CounterclockwiseProcess
	,&PlayerMoveScene::ClockwiseProcess
	,&PlayerMoveScene::MouseSetAimedUnitProcess
	,&PlayerMoveScene::ItemProcess
	,&PlayerMoveScene::WaitProcess
	,&PlayerMoveScene::CancelProcess
	,&PlayerMoveScene::ResearchProcess
	,&PlayerMoveScene::SystemMenuProcess
	,&PlayerMoveScene::MoveProcess
};

std::pair<bool,int> PlayerMoveScene::AttackProcess(){
	const Vector2D mousePos=GetMousePointVector2D();
	if(JudgeAttackCommandUsable()
		&& (keyboard_get(KEY_INPUT_Z)==1
			|| (m_aimedUnit->GetUnitCircleShape()->VJudgePointInsideShape(mousePos)
				//&& mouse_get(MOUSE_INPUT_LEFT)==1
				&& BattleSceneData::JudgeMousePushInsideMapDrawZone(MOUSE_INPUT_LEFT,false)
				)
			)
		&& m_attackableOnlyChangeInherit
		)
	{
		//�U��
		//�U���Ώۂ����݂��AOP������Ă���ꍇ�̂ݍU���������s��
		//FinishUnitOperation();//�s���I������(���Ƃ�)
		return std::pair<bool,int>(true,SceneKind::e_attackNormal);//�U����ʂ�
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::SkillProcess(){
	if(keyboard_get(KEY_INPUT_D)==1
		)
	{
		//�K�E�Z
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::CounterclockwiseProcess(){
	if(keyboard_get(KEY_INPUT_A)==1
		&& JudgeAttackCommandUsable()
		&& m_attackableOnlyChangeInherit
		)
	{
		//�U���R�}���h�g�p�\�̎��̂݁A�_���̃L�����̕ύX(�����v���)
		SetAimedUnit(-1);
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);//�I���̌��ʉ��Đ�
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::ClockwiseProcess(){
	if(keyboard_get(KEY_INPUT_S)==1
		&& JudgeAttackCommandUsable()
		&& m_attackableOnlyChangeInherit
		)
	{
		//�U���R�}���h�g�p�\�̎��̂݁A�_���̃L�����̕ύX(���v���)
		SetAimedUnit(1);
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);//�I���̌��ʉ��Đ�
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::MouseSetAimedUnitProcess(){
	const Vector2D mousePos=GetMousePointVector2D();
	if((m_mousePosJustBefore-mousePos).sqSize()>=1.0f
		&& JudgeAttackCommandUsable()
		&& JudgeBecomeAimedUnit(m_battleSceneData->GetUnitPointer(mousePos))
		&& m_attackableOnlyChangeInherit
		)
	{
		//�U���R�}���h�g�p�\�̎��Ƀ}�E�X��傫�����������Ƃ��̂݁A�_���̃L�����̕ύX
		const Unit *beforeAimedUnit=m_aimedUnit;
		m_aimedUnit=m_battleSceneData->GetUnitPointer(mousePos);
		if(m_aimedUnit!=nullptr && m_aimedUnit!=beforeAimedUnit){
			//�I�����j�b�g���ύX����Ă���Α_���؂�ւ�����炷
			PlaySoundMem(m_battleSceneData->m_aimchangeSound,DX_PLAYTYPE_BACK,TRUE);
		}
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::ItemProcess(){
	if(keyboard_get(KEY_INPUT_C)==1){
		//�A�C�e���̎g�p
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::WaitProcess(){
	if(
		(
			keyboard_get(KEY_INPUT_V)==1
			|| mouse_get(MOUSE_INPUT_MIDDLE)==1
			|| m_waitButton.JudgePressMoment()
			)
		&& m_waitableOnlyChangeInherit
		)
	{
		//�ҋ@
		FinishUnitOperation();
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//�ҋ@����͌��艹
		return std::pair<bool,int>(true,0);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::CancelProcess(){
	if(
		(
			keyboard_get(KEY_INPUT_X)==1
			|| keyboard_get(KEY_INPUT_X)>30
			|| mouse_get(MOUSE_INPUT_RIGHT)==1
			|| mouse_get(MOUSE_INPUT_RIGHT)>30
			)
		&& m_moveableOnlyChangeInherit
		)
	{
		//�ړ���蒼��(m_route.back()��1�O�̏ꏊ�ɖ߂��Bback()�̈ʒu�͌��݈ʒu�̉\������������)
		if(!m_route.empty()){
			m_route.pop_back();
			if(!m_route.empty()){
				//�����v�f���Ȃ����pop_back()�����Ȃ�
				RouteInfo info=m_route.back();
				m_route.pop_back();
				//���j�b�g���ړ�������
				m_battleSceneData->m_operateUnit->Warp(info.pos);
				//OP���񕜂�����
				//m_battleSceneData->m_operateUnit->AddOP(info.OP-m_battleSceneData->m_operateUnit->GetBattleStatus().OP);
				m_battleSceneData->m_operateUnit->SetOP(info.OP);
			}
			//�ʒu�X�V���s��
			PositionUpdate(Vector2D());
		}
		//�X�R�A�V�X�e������
		m_battleSceneData->m_scoreObserver->CancelUpdate();
		//�L�����Z�������������̂ŁAtrue��Ԃ�
		return std::pair<bool,int>(true,SceneKind::e_move);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::ResearchProcess(){
	if(keyboard_get(KEY_INPUT_F)==1 || m_researchButton.JudgePressMoment()){
		//�}�b�v���׃��[�h��
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
		return std::pair<bool,int>(true,SceneKind::e_research);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::SystemMenuProcess(){
	if(keyboard_get(KEY_INPUT_SPACE)==1 || m_menuButton.JudgePressMoment()){
		//�V�X�e�����j���[��ʂ�
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
		return std::pair<bool,int>(true,SceneKind::e_systemMenu);
	}
	return std::pair<bool,int>(false,SceneKind::e_move);
}

std::pair<bool,int> PlayerMoveScene::MoveProcess(){
	if(m_moveableOnlyChangeInherit){
		//�ړ����n�߂̔���X�V(���N���b�N���������u�Ԃł��邩�𔻒�E�L�^����)
		int frame=mouse_get(MOUSE_INPUT_LEFT);
		if(frame==0){
			//���N���b�N�����Ȃ��Ȃ�ƁA�����Ă��Ȃ��̂�false�ɂ���
			m_mouseLeftFlag=false;
		} else if(frame==1){
			//1�t���[���ڂ����m�ł����ꍇ�́Atrue�ɂ���
			m_mouseLeftFlag=true;
		}
		//�ړ�����
		if(PositionUpdate(CalculateInputVec())){
			//�L�[���͂��Ȃ����m_operateUnit�̈ʒu�X�V
			//�ʒu�X�V���������̏���
		}
	}
	return std::pair<bool,int>(true,SceneKind::e_move);
}

void PlayerMoveScene::EnableAllChangeInherit(){
	m_moveableOnlyChangeInherit=true;
	m_attackableOnlyChangeInherit=true;
	m_waitableOnlyChangeInherit=true;
}

PlayerMoveScene::PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData)
	,m_waitButton(1520,980,80,80,LoadGraphEX(FilePath::graphicDir+"nextButton.png"))
	,m_researchButton(1620,980,80,80,LoadGraphEX(FilePath::graphicDir+"researchButton.png"))
	,m_menuButton(1820,980,80,80,LoadGraphEX(FilePath::graphicDir+"menuButton.png"))
	,m_mousePosJustBefore(GetMousePointVector2D())
	,m_mouseLeftFlag(false)
	,m_attackableOnlyChangeInherit(true)
	,m_moveableOnlyChangeInherit(true)
	,m_waitableOnlyChangeInherit(true)
	,m_submissionPosition(GetSubmissionX(),-SelfDecideSubmission::s_submissionHeight,10,Easing::TYPE_IN,Easing::FUNCTION_LINER,0.0)
	,m_notOperateFrame(0)
{}

Vector2D PlayerMoveScene::CalculateInputVec()const{
	//�ړ�����
	Vector2D moveVec;
	if(//mouse_get(MOUSE_INPUT_LEFT)>0
		BattleSceneData::JudgeMousePushInsideMapDrawZone(MOUSE_INPUT_LEFT,true)
		&& m_mouseLeftFlag
		)
	{
		//�}�E�X�����N���b�N���Ă���Ȃ�}�E�X���͂ɑΉ�������
		moveVec=GetMousePointVector2D()-m_battleSceneData->m_operateUnit->getPos();
		//�A�i���O�X�e�B�b�N�̕����I�ȃY�����ɂ��moveVec�̔����͂�����
		const float gap=4.0f;
		if(std::abs(moveVec.x)<gap){
			moveVec.x=0.0f;
		}
		if(std::abs(moveVec.y)<gap){
			moveVec.y=0.0f;
		}
	} else if(analogjoypad_get(DX_INPUT_PAD1)!=Vector2D()){
		//�A�i���O�X�e�B�b�N���͂�����΂����p����
		moveVec=analogjoypad_get(DX_INPUT_PAD1);
		//�A�i���O�X�e�B�b�N�̕����I�ȃY�����ɂ��moveVec�̔����͂�����
		const float gap=50.0f;
		if(std::abs(moveVec.x)<gap){
			moveVec.x=0.0f;
		}
		if(std::abs(moveVec.y)<gap){
			moveVec.y=0.0f;
		}
	} else{
		//�L�[�{�[�h���͂��Ă���Ȃ�A�����p����
		moveVec=Vector2D();
		if(keyboard_get(KEY_INPUT_UP)>0){
			moveVec+=Vector2D(0.0f,-100.0f);
		}
		if(keyboard_get(KEY_INPUT_LEFT)>0){
			moveVec+=Vector2D(-100.0f,0.0f);
		}
		if(keyboard_get(KEY_INPUT_RIGHT)>0){
			moveVec+=Vector2D(100.0f,0.0f);
		}
		if(keyboard_get(KEY_INPUT_DOWN)>0){
			moveVec+=Vector2D(0.0f,100.0f);
		}
	}
	return moveVec;
}

int PlayerMoveScene::thisCalculate(){
	//�f�o�b�O�R�}���h�̓���
	if(keyboard_get(KEY_INPUT_LSHIFT)==60){
		m_battleSceneData->m_drawObjectShapeFlag=!m_battleSceneData->m_drawObjectShapeFlag;
	}

	//�L�[���͎�t
	const Vector2D mousePos=GetMousePointVector2D();

	std::pair<bool,int> retPal;
	for(const std::function<std::pair<bool,int>(PlayerMoveScene&)> &func:inCalculateProcessFunction){
		retPal=func(*this);
		if(retPal.first && retPal.second!=SceneKind::e_move){
			return retPal.second;
		}
	}

/*
	if(JudgeAttackCommandUsable()
		&& (keyboard_get(KEY_INPUT_Z)==1
			|| (m_aimedUnit->GetUnitCircleShape()->VJudgePointInsideShape(mousePos)
				//&& mouse_get(MOUSE_INPUT_LEFT)==1
				&& BattleSceneData::JudgeMousePushInsideMapDrawZone(MOUSE_INPUT_LEFT,false)
				)
			)
		)
	{
		//�U��
		//�U���Ώۂ����݂��AOP������Ă���ꍇ�̂ݍU���������s��
		//FinishUnitOperation();//�s���I������(���Ƃ�)
		return SceneKind::e_attackNormal;//�U����ʂ�
	} else if(keyboard_get(KEY_INPUT_D)==1){
		//�K�E�Z

	} else if(keyboard_get(KEY_INPUT_A)==1 && JudgeAttackCommandUsable()){
		//�U���R�}���h�g�p�\�̎��̂݁A�_���̃L�����̕ύX(�����v���)
		SetAimedUnit(-1);
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);//�I���̌��ʉ��Đ�
	} else if(keyboard_get(KEY_INPUT_S)==1 && JudgeAttackCommandUsable()){
		//�U���R�}���h�g�p�\�̎��̂݁A�_���̃L�����̕ύX(���v���)
		SetAimedUnit(1);
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);//�I���̌��ʉ��Đ�
	} else if((m_mousePosJustBefore-mousePos).sqSize()>=1.0f
		&& JudgeAttackCommandUsable()
		&& JudgeBecomeAimedUnit(m_battleSceneData->GetUnitPointer(mousePos))
		)
	{
		//�U���R�}���h�g�p�\�̎��Ƀ}�E�X��傫�����������Ƃ��̂݁A�_���̃L�����̕ύX
		const Unit *beforeAimedUnit=m_aimedUnit;
		m_aimedUnit=m_battleSceneData->GetUnitPointer(mousePos);
		if(m_aimedUnit!=nullptr && m_aimedUnit!=beforeAimedUnit){
			//�I�����j�b�g���ύX����Ă���Α_���؂�ւ�����炷
			PlaySoundMem(m_battleSceneData->m_aimchangeSound,DX_PLAYTYPE_BACK,TRUE);
		}
	} else if(keyboard_get(KEY_INPUT_C)==1){
		//�A�C�e���̎g�p

	} else if(keyboard_get(KEY_INPUT_V)==1 || m_waitButton.JudgePressMoment()){
		//�ҋ@
		FinishUnitOperation();
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//�ҋ@����͌��艹
		return 0;
	} else if(keyboard_get(KEY_INPUT_X)==1
		|| keyboard_get(KEY_INPUT_X)>30
		|| mouse_get(MOUSE_INPUT_RIGHT)==1
		|| mouse_get(MOUSE_INPUT_RIGHT)>30
		)
	{
		//�ړ���蒼��(m_route.back()��1�O�̏ꏊ�ɖ߂��Bback()�̈ʒu�͌��݈ʒu�̉\������������)
		if(!m_route.empty()){
			m_route.pop_back();
			if(!m_route.empty()){
				//�����v�f���Ȃ����pop_back()�����Ȃ�
				RouteInfo info=m_route.back();
				m_route.pop_back();
				//���j�b�g���ړ�������
				m_battleSceneData->m_operateUnit->Warp(info.pos);
				//OP���񕜂�����
				//m_battleSceneData->m_operateUnit->AddOP(info.OP-m_battleSceneData->m_operateUnit->GetBattleStatus().OP);
				m_battleSceneData->m_operateUnit->SetOP(info.OP);
			}
			//�ʒu�X�V���s��
			PositionUpdate(Vector2D());
		}
	} else if(keyboard_get(KEY_INPUT_F)==1 || m_researchButton.JudgePressMoment()){
		//�}�b�v���׃��[�h��
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
		return SceneKind::e_research;
	} else if(keyboard_get(KEY_INPUT_SPACE)==1 || m_menuButton.JudgePressMoment()){
		//�V�X�e�����j���[��ʂ�
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
		return SceneKind::e_systemMenu;
	} else{
		//�ړ����n�߂̔���X�V(���N���b�N���������u�Ԃł��邩�𔻒�E�L�^����)
		int frame=mouse_get(MOUSE_INPUT_LEFT);
		if(frame==0){
			//���N���b�N�����Ȃ��Ȃ�ƁA�����Ă��Ȃ��̂�false�ɂ���
			m_mouseLeftFlag=false;
		} else if(frame==1){
			//1�t���[���ڂ����m�ł����ꍇ�́Atrue�ɂ���
			m_mouseLeftFlag=true;
		}
		//�ړ�����
		if(PositionUpdate(CalculateInputVec())){
			//�L�[���͂��Ȃ����m_operateUnit�̈ʒu�X�V
			//�ʒu�X�V���������̏���
		}
	}
//*/

	//�����쎞�Ԃ̍X�V(�}�E�X�݂̂�)
	if(mousePos!=m_mousePosJustBefore){
		m_notOperateFrame=0;
	} else{
		m_notOperateFrame++;
	}
	printfDx("frame:%d y:%d targety:%d\n",m_notOperateFrame,m_submissionPosition.GetY(),m_submissionPosition.GetendY());

	//�T�u�~�b�V�����̈ʒu�̍X�V
	if(m_notOperateFrame==0){
		//���삳�ꂽ��A��ʊO�Ɉړ�������
		//�����������Ă��O�Ɉړ����Ȃ��Ȃ��Ă��܂����߁Abool�l�͍H�v����
		m_submissionPosition.SetTarget(GetSubmissionX(),-SelfDecideSubmission::s_submissionHeight,m_submissionPosition.GetendY()!=-SelfDecideSubmission::s_submissionHeight);
	} else if(m_notOperateFrame==120){
		//2�b�������Ԃ����������ʓ��Ɉړ�������
		m_submissionPosition.SetTarget(GetSubmissionX(),0,true);
	} else{
		m_submissionPosition.Update();
	}

	//���t���[���ɁA�{�t���[���ɂ�����}�E�X�̈ʒu��������悤�ɂ���
	m_mousePosJustBefore=mousePos;
	
	return SceneKind::e_move;	
}

void PlayerMoveScene::thisDraw()const{
	//MoveScene�̂����̂܂�
	MoveScene::thisDraw();

	//�{�^����`��
	m_waitButton.DrawButton();
	m_researchButton.DrawButton();
	m_menuButton.DrawButton();

	//�T�u�~�b�V�����`��
	if(m_battleSceneData->m_submissionRunFlag){
		m_battleSceneData->m_scoreObserver->GetSubmission().DrawSubmission(m_submissionPosition.GetX(),m_submissionPosition.GetY());
	}
}

void PlayerMoveScene::ReturnProcess(){
	//MoveScene��ReturnProcess�ɉ����Am_mouseLeftFlag������������
	MoveScene::ReturnProcess();
	m_mouseLeftFlag=false;
}
