#include"input.h"
#include"DxLib.h"
#include"PlayerMoveScene.h"

//----------------------PlayerMoveScene------------------------
PlayerMoveScene::PlayerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData){}

Vector2D PlayerMoveScene::CalculateInputVec()const{
	Vector2D moveVec;
	if(mouse_get(MOUSE_INPUT_LEFT)>0){
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
	} else{
		//�ʏ�̓A�i���O�X�e�B�b�N����
		moveVec=analogjoypad_get(DX_INPUT_PAD1);
		//�A�i���O�X�e�B�b�N�̕����I�ȃY�����ɂ��moveVec�̔����͂�����
		const float gap=50.0f;
		if(std::abs(moveVec.x)<gap){
			moveVec.x=0.0f;
		}
		if(std::abs(moveVec.y)<gap){
			moveVec.y=0.0f;
		}
	}
	return moveVec;
}

int PlayerMoveScene::thisCalculate(){
	//�f�o�b�O�R�}���h�̓���
	if(keyboard_get(KEY_INPUT_LSHIFT)==60){
		m_battleSceneData->m_drawObjectShapeFlag=!m_battleSceneData->m_drawObjectShapeFlag;
	}

	//m_operateUnit�̈ʒu�X�V
	if(PositionUpdate(CalculateInputVec())){
		//�ʒu�X�V���������̏���

	} else{
		//�ړ���������Ȃ��������͂��̑��̓��͂��󂯕t����
		if(keyboard_get(KEY_INPUT_Z)==1){
			//�U��
			if(JudgeAttackCommandUsable()){
				//�U���Ώۂ����݂��AOP������Ă���ꍇ�̂ݍU���������s��
				//FinishUnitOperation();//�s���I������(���Ƃ�)
				return SceneKind::e_attackNormal;//�U����ʂ�
			}
		} else if(keyboard_get(KEY_INPUT_D)==1){
			//�K�E�Z

		} else if(keyboard_get(KEY_INPUT_A)==1 && JudgeAttackCommandUsable()){
			//�U���R�}���h�g�p�\�̎��̂݁A�_���̃L�����̕ύX(�����v���)
			SetAimedUnit(-1);
		} else if(keyboard_get(KEY_INPUT_S)==1 && JudgeAttackCommandUsable()){
			//�U���R�}���h�g�p�\�̎��̂݁A�_���̃L�����̕ύX(���v���)
			SetAimedUnit(1);
		} else if(keyboard_get(KEY_INPUT_C)==1){
			//�A�C�e���̎g�p

		} else if(keyboard_get(KEY_INPUT_V)==1){
			//�ҋ@
			FinishUnitOperation();
			return 0;
		} else if(keyboard_get(KEY_INPUT_X)==1 || keyboard_get(KEY_INPUT_X)>30){
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
		} else if(keyboard_get(KEY_INPUT_F)==1){
			//�}�b�v���׃��[�h��
			return SceneKind::e_research;
		}
	}
	return SceneKind::e_move;
}
