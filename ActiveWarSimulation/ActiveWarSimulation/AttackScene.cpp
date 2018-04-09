#include"DxLib.h"
#include"AttackScene.h"
#include"GraphicControl.h"

//-------------------AttackScene-------------------
const int AttackScene::motionFlame=120;
const int AttackScene::damageBeginFlame=50;
const int AttackScene::damageEndFlame=80;

AttackScene::AttackScene(std::shared_ptr<BattleSceneData> data,Unit *aimedUnit)
	:BattleSceneElement(SceneKind::e_attackNormal)
	,m_battleSceneData(data),m_aimedUnit(aimedUnit)
	,m_attackMotion({
		PositionControl(data->m_operateUnit->getPos(),m_aimedUnit->getPos(),motionFlame/2,Easing::TYPE_OUT,Easing::FUNCTION_QUAD,1.0)
		,PositionControl(m_aimedUnit->getPos(),data->m_operateUnit->getPos(),motionFlame/2,Easing::TYPE_IN,Easing::FUNCTION_QUAD,1.0)
	})
	,m_damageMotion(0,0,0,-(int)Unit::unitCircleSize,damageEndFlame-damageBeginFlame,Easing::TYPE_OUT,Easing::FUNCTION_QUAD,9.0)
{
	//�U�����̋L�^
	m_attackinfo=m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_battleSceneData->m_operateUnit,m_aimedUnit);
	m_damageFont=CreateFontToHandleEX("���C���I",30,4,DX_FONTTYPE_ANTIALIASING_EDGE);
}

AttackScene::~AttackScene(){
	DeleteFontToHandleEX(m_damageFont);
}

int AttackScene::thisCalculate(){
	//�A�j���[�V�����X�V
	m_attackMotion.Update();
	m_damageMotion.Update();
	const int flame=m_attackMotion.GetFlame();
	//�_���[�W���[�V�����̊J�n
	if(flame==damageBeginFlame){
		//�A�j���[�V�����̏�����
		m_damageMotion.Retry();
		//�_���[�W����

	}
	//�I������
	if(m_attackMotion.GetEndFlag() && m_damageMotion.GetEndFlag()){
		//�ЂƂ܂����O��ʂ�
		return -1;
	}

	return SceneKind::e_attackNormal;
}

void AttackScene::thisDraw()const{
	//�t�B�[���h�̕`��
	m_battleSceneData->DrawField(std::set<const BattleObject *>{m_battleSceneData->m_operateUnit,m_aimedUnit});

	//�U���Ώۃ��j�b�g�̕`��
	if(m_aimedUnit!=nullptr){
		m_aimedUnit->BattleObject::VDraw();
	}
	
	//�U�������s���郆�j�b�g�̕`��
	m_battleSceneData->m_operateUnit->VDraw(Vector2D((float)m_attackMotion.GetX(),(float)m_attackMotion.GetY()),Vector2D());
	
	//�S���j�b�g��HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();

	//���j�b�g�̃I�[�_�[���Ԃ�`��
	m_battleSceneData->DrawOrder();

	//�_���[�W�̕`��
	if(m_attackMotion.GetFlame()>damageBeginFlame){
		//�\������t���[�����O�͕`�揈�����s��Ȃ�
		//�F�̌���
		unsigned int incolor,outcolor;
		if(m_attackinfo.damage<=0){
			//�m�[�_���[�W��񕜂̏ꍇ�͗΂ŕ`��
			incolor=GetColor(64,255,64);
			outcolor=GetColor(0,0,0);
		} else{
			//�_���[�W�͐Ԃ��`��
			incolor=GetColor(255,0,0);
			outcolor=GetColor(255,255,255);
		}
		//�`��
		const int x=(int)m_aimedUnit->getPos().x+m_damageMotion.GetX(),y=(int)m_aimedUnit->getPos().y+m_damageMotion.GetY();
		DrawStringCenterBaseToHandle(x,y,std::to_string(std::abs(m_attackinfo.damage)).c_str(),incolor,m_damageFont,true,outcolor);
	}
}

int AttackScene::UpdateNextScene(int index){
	//���ɉ������Ȃ�
	return index;
}
