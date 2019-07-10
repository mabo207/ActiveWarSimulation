#include"DxLib.h"
#include"AttackScene.h"
#include"GraphicControl.h"
#include"FilePath.h"

//-------------------AttackScene-------------------
const float AttackScene::moveLength=40.0;

AttackScene::AttackScene(const std::shared_ptr<BattleSceneData> &data,Unit *aimedUnit)
	:DamageScene(SceneKind::e_attackNormal
		,data
		,SceneKind::e_switch
		,aimedUnit
		,data->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(data->m_operateUnit,aimedUnit))
	,m_attackMotion({
		PositionControl(data->m_operateUnit->getPos(),data->m_operateUnit->getPos()+(m_aimedUnit->getPos()-data->m_operateUnit->getPos()).norm()*moveLength,motionFrame/2,Easing::TYPE_OUT,Easing::FUNCTION_LINER,1.0)
		,PositionControl(data->m_operateUnit->getPos()+(m_aimedUnit->getPos()-data->m_operateUnit->getPos()).norm()*moveLength,data->m_operateUnit->getPos(),motionFrame/2,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0)
	})
{
	//�G�t�F�N�g�̍쐬
	LoadDivGraph((FilePath::graphicDir+"effect/zangeki.png").c_str(),30,30,1,100,100,m_effect);
}

AttackScene::~AttackScene(){
	//�G�t�F�N�g�̉��
	for(size_t i=0;i<30;i++){
		DeleteGraph(m_effect[i]);
	}
}

void AttackScene::AnimationUpdate(){
	DamageScene::AnimationUpdate();//�_���[�W�����̃A�j���[�V�����X�V
	m_attackMotion.Update();//�U�����j�b�g�̃A�j���[�V�����X�V
}

void AttackScene::DrawUnit()const{
	//���j�b�g�̕`��
	m_battleSceneData->DrawUnit(false,std::set<const Unit *>{m_battleSceneData->m_operateUnit,m_aimedUnit});
	//�U���Ώۃ��j�b�g�̕`��
	if(m_aimedUnit!=nullptr){
		m_aimedUnit->DrawUnit(Vector2D(),m_battleSceneData->m_fpsMesuring.GetFrame(),false,false,false);
	}
	//�U�������s���郆�j�b�g�̕`��
	m_battleSceneData->m_operateUnit->DrawUnit(Vector2D((float)m_attackMotion.GetX(),(float)m_attackMotion.GetY()),Vector2D(),m_battleSceneData->m_fpsMesuring.GetFrame(),false,false,false);
}

void AttackScene::DrawAnimation()const{
	//�G�t�F�N�g�̕`��
	if(m_attackMotion.GetFrame()<30){
		//DrawGraph(((int)m_aimedUnit->getPos().x)-50,((int)m_aimedUnit->getPos().y)-50,m_effect[m_attackMotion.GetFrame()],TRUE);
	}
	//�_���[�W�����̕`��
	DamageScene::DrawAnimation();
}

bool AttackScene::JudgeAnimationEnd()const{
	return (DamageScene::JudgeAnimationEnd() && m_attackMotion.GetEndFlag());
}

void AttackScene::DamageProcess(){
	//�R�X�g�̏���
	//�U�����s���ɍU������OP�������d�l��ς���ꍇ�́A�g�y�ӏ����l������i�X�R�A�̌v�Z�Ȃǂ͂��̎d�l��O��Ƃ������������Ă���j
	//m_battleSceneData->m_operateUnit->AddOP(m_battleSceneData->m_operateUnit->CalculateAddOPNormalAttack());
	m_battleSceneData->m_operateUnit->ConsumeOPByCost(m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetCost());
	//���샆�j�b�g���Ώۃ��j�b�g�ւ̍U������
	DamageScene::DamageProcess();
	//�Ώۃ��j�b�g�������c���Ă���Ȃ甽���������s��
	//������
}

int AttackScene::UpdateNextScene(int index){
	//���ɉ������Ȃ�
	return index;
}

void AttackScene::ReturnProcess(){}
