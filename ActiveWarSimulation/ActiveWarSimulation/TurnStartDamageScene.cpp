#include"DxLib.h"
#include"TurnStartDamageScene.h"
#include"GraphicControl.h"
#include"FilePath.h"

//--------------------TurnStartDamageScene-------------------
TurnStartDamageScene::TurnStartDamageScene(const std::shared_ptr<BattleSceneData> &data)
	:DamageScene(SceneKind::e_turnStartDamage
		,data
		,SceneKind::e_move
		,data->m_operateUnit
		,GetAttackInfo(data))
{
	//�G�t�F�N�g�̍쐬
	LoadDivGraph((FilePath::graphicDir+"effect/zangeki.png").c_str(),30,30,1,100,100,m_effect);
}

TurnStartDamageScene::~TurnStartDamageScene(){
	//�G�t�F�N�g�̉��
	for(size_t i=0;i<30;i++){
		DeleteGraph(m_effect[i]);
	}
}

bool TurnStartDamageScene::JudgeTurnStartDamage(const std::shared_ptr<BattleSceneData> &data){
	return GetAttackInfo(data).damage!=0;
}

Weapon::AttackInfo TurnStartDamageScene::GetAttackInfo(const std::shared_ptr<BattleSceneData> &data){
	Weapon::AttackInfo info;
	info.damage=1;
	return info;
}

void TurnStartDamageScene::DrawAnimation()const{
	//�G�t�F�N�g�̕`��
	if(m_frame<30){
		//DrawGraph(((int)m_aimedUnit->getPos().x)-50,((int)m_aimedUnit->getPos().y)-50,m_effect[m_attackMotion.GetFrame()],TRUE);
	}
	//�_���[�W�����̕`��
	DamageScene::DrawAnimation();
}

void TurnStartDamageScene::DamageProcess(){
	//�_���[�W����
	DamageScene::DamageProcess();
	//m_aimedUnit���S��
	if(m_aimedUnit->GetBattleStatus().HP<=0){
		m_nextSceneKind=SceneKind::e_switch;
	}
}

int TurnStartDamageScene::UpdateNextScene(int index){
	//���ɉ������Ȃ�
	return index;
}

void TurnStartDamageScene::ReturnProcess(){}
