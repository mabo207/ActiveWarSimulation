#include"DxLib.h"
#include"PlayerMoveScene.h"
#include"GraphicControl.h"

//----------------------PlayerMoveScene------------------------
PlayerMoveScene::PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData)
	,m_waitButton(1520,980,80,80,LoadGraphEX("Graphic/nextButton.png"))
	,m_researchButton(1620,980,80,80,LoadGraphEX("Graphic/researchButton.png"))
	,m_mousePosJustBefore(GetMousePointVector2D())
	,m_mouseLeftFlag(false)
{}

Vector2D PlayerMoveScene::CalculateInputVec()const{
	//�ړ�����
	Vector2D moveVec;
	if(mouse_get(MOUSE_INPUT_LEFT)>0 && m_mouseLeftFlag){
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
	if(JudgeAttackCommandUsable()
		&& (keyboard_get(KEY_INPUT_Z)==1
			|| (m_aimedUnit->GetUnitCircleShape()->VJudgePointInsideShape(mousePos) && mouse_get(MOUSE_INPUT_LEFT)==1)
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
	} else if(keyboard_get(KEY_INPUT_S)==1 && JudgeAttackCommandUsable()){
		//�U���R�}���h�g�p�\�̎��̂݁A�_���̃L�����̕ύX(���v���)
		SetAimedUnit(1);
	} else if((m_mousePosJustBefore-mousePos).sqSize()>=1.0f
		&& JudgeAttackCommandUsable()
		&& JudgeBecomeAimedUnit(m_battleSceneData->GetUnitPointer(mousePos))
		)
	{
		//�U���R�}���h�g�p�\�̎��Ƀ}�E�X��傫�����������Ƃ��̂݁A�_���̃L�����̕ύX
		m_aimedUnit=m_battleSceneData->GetUnitPointer(mousePos);
	} else if(keyboard_get(KEY_INPUT_C)==1){
		//�A�C�e���̎g�p

	} else if(keyboard_get(KEY_INPUT_V)==1 || m_waitButton.JudgePressMoment()){
		//�ҋ@
		FinishUnitOperation();
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
		return SceneKind::e_research;
	} else{
		//�ړ����n�߂̔���X�V(���N���b�N���������u�Ԃł��邩�𔻒�E�L�^����)
		int flame=mouse_get(MOUSE_INPUT_LEFT);
		if(flame==0){
			//���N���b�N�����Ȃ��Ȃ�ƁA�����Ă��Ȃ��̂�false�ɂ���
			m_mouseLeftFlag=false;
		} else if(flame==1){
			//1�t���[���ڂ����m�ł����ꍇ�́Atrue�ɂ���
			m_mouseLeftFlag=true;
		}
		//�ړ�����
		if(PositionUpdate(CalculateInputVec())){
			//�L�[���͂��Ȃ����m_operateUnit�̈ʒu�X�V
			//�ʒu�X�V���������̏���
		}
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
}

void PlayerMoveScene::ReturnProcess(){
	//MoveScene��ReturnProcess�ɉ����Am_mouseLeftFlag������������
	MoveScene::ReturnProcess();
	m_mouseLeftFlag=false;
}
