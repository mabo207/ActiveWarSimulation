#include"DxLib.h"
#include"SwitchUnitScene.h"
#include"PlayerMoveScene.h"
#include"ComputerMoveScene.h"

//------------------SwitchUnitScene-------------------
SwitchUnitScene::SwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_switch),m_battleSceneData(battleSceneData)
{
	m_battleSceneData->m_fpsMesuring.RecordTime();//�^�C�}�[������
	m_battleSceneData->FinishUnitOperation();//���j�b�g�̐؂�ւ�����
}

SwitchUnitScene::~SwitchUnitScene(){}

int SwitchUnitScene::thisCalculate(){
	//�^�C�}�[�X�V
	m_battleSceneData->m_fpsMesuring.Update();
	//�y�ЂƂ܂��z��莞�Ԍo�����烆�j�b�g�ړ���
	if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0f){
		//���j�b�g�ڍs������
		return SceneKind::e_move;
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
}

int SwitchUnitScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_move):
		if(m_battleSceneData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
			//�v���C���[�L�����Ȃ玩����N���X��
			m_nextScene=std::shared_ptr<BattleSceneElement>(new PlayerMoveScene(m_battleSceneData));
		} else{
			//����ȊO�Ȃ�AI����N���X��
			m_nextScene=std::shared_ptr<BattleSceneElement>(new ComputerMoveScene(m_battleSceneData));
		}
		return index;
	default:
		return index;
	}
}

void SwitchUnitScene::ReturnProcess(){
	m_battleSceneData->m_fpsMesuring.RecordTime();//�^�C�}�[������
	m_battleSceneData->FinishUnitOperation();//���j�b�g�̐؂�ւ�����
}
