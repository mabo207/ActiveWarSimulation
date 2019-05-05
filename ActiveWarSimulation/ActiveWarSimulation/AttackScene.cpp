#include"DxLib.h"
#include"AttackScene.h"
#include"GraphicControl.h"

//-------------------AttackScene-------------------
const int AttackScene::motionFrame=20;
const int AttackScene::damageBeginFrame=AttackScene::motionFrame/2;
const int AttackScene::damageEndFrame=AttackScene::damageBeginFrame+30;
const float AttackScene::moveLength=40.0;

AttackScene::AttackScene(std::shared_ptr<BattleSceneData> data,Unit *aimedUnit)
	:BattleSceneElement(SceneKind::e_attackNormal)
	,m_battleSceneData(data),m_aimedUnit(aimedUnit)
	,m_attackMotion({
		PositionControl(data->m_operateUnit->getPos(),data->m_operateUnit->getPos()+(m_aimedUnit->getPos()-data->m_operateUnit->getPos()).norm()*moveLength,motionFrame/2,Easing::TYPE_OUT,Easing::FUNCTION_LINER,1.0)
		,PositionControl(data->m_operateUnit->getPos()+(m_aimedUnit->getPos()-data->m_operateUnit->getPos()).norm()*moveLength,data->m_operateUnit->getPos(),motionFrame/2,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0)
	})
	,m_damageMotion({
		PositionControl(0,0,0,-(int)Unit::unitCircleSize,(damageEndFrame-damageBeginFrame)/2,Easing::TYPE_OUT,Easing::FUNCTION_EXPO,9.0)
		,PositionControl(0,0,-(int)Unit::unitCircleSize,-(int)Unit::unitCircleSize,(damageEndFrame-damageBeginFrame)/2,Easing::TYPE_OUT,Easing::FUNCTION_QUAD,9.0)
	})
{
	//�U�����̋L�^
	m_attackinfo=m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_battleSceneData->m_operateUnit,m_aimedUnit);
	m_damageFont=CreateFontToHandleEX("���C���I",30,4,DX_FONTTYPE_ANTIALIASING_EDGE);

	LoadDivGraph("Graphic/effect/zangeki.png",30,30,1,100,100,m_effect);
}

AttackScene::~AttackScene(){
	DeleteFontToHandleEX(m_damageFont);

	for(size_t i=0;i<30;i++){
		DeleteGraph(m_effect[i]);
	}
}

void AttackScene::ProcessAttack(){
	//�R�X�g�̏���
	//�U�����s���ɍU������OP�������d�l��ς���ꍇ�́A�g�y�ӏ����l������i�X�R�A�̌v�Z�Ȃǂ͂��̎d�l��O��Ƃ������������Ă���j
	//m_battleSceneData->m_operateUnit->AddOP(m_battleSceneData->m_operateUnit->CalculateAddOPNormalAttack());
	m_battleSceneData->m_operateUnit->ConsumeOPByCost(m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetCost());
	//���샆�j�b�g���Ώۃ��j�b�g�ւ̍U�����̌v�Z
	Weapon::AttackInfo attackinfo=m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_battleSceneData->m_operateUnit,m_aimedUnit);
	//���샆�j�b�g���Ώۃ��j�b�g�ւ̍U������
	int aimedHP=m_aimedUnit->AddHP(-attackinfo.damage);
	//���ʉ���炷
	if(attackinfo.damage>0){
		//�_���[�W������΍U����
		PlaySoundMem(m_battleSceneData->m_attackSound,DX_PLAYTYPE_BACK,TRUE);
	} else{
		//�񕜂��Ă���Ȃ�񕜉�
		PlaySoundMem(m_battleSceneData->m_healSound,DX_PLAYTYPE_BACK,TRUE);
	}
	if(aimedHP<=0){
		//�Ώۃ��j�b�g��HP��0�ȉ��Ȃ�A�X�e�[�W���烆�j�b�g����菜��
		m_aimedUnit->SetFix(Shape::Fix::e_ignore);//�����蔻��̑Ώۂ����菜��
		//m_unitList����m_aimedUnit����菜��
		for(std::vector<Unit *>::const_iterator it=m_battleSceneData->m_unitList.begin(),ite=m_battleSceneData->m_unitList.end();it!=ite;it++){
			if(*it==m_aimedUnit){
				m_battleSceneData->m_unitList.erase(it);
				break;
			}
		}
	} else{
		//�Ώۃ��j�b�g�������c���Ă���Ȃ甽���������s��
		//������
	}
}

int AttackScene::thisCalculate(){
	//�A�j���[�V�����X�V
	m_attackMotion.Update();
	m_damageMotion.Update();
	const int frame=m_attackMotion.GetFrame();
	//�_���[�W���[�V�����̊J�n
	if(frame==damageBeginFrame){
		//�A�j���[�V�����̏�����
		m_damageMotion.Retry();
		//�_���[�W����
		ProcessAttack();
	}
	//�I������
	if(m_attackMotion.GetEndFlag() && m_damageMotion.GetEndFlag()){
		//���j�b�g�؂�ւ���ʂ�
		return SceneKind::e_switch;
	}

	return SceneKind::e_attackNormal;
}

void AttackScene::thisDraw()const{
	//�t�B�[���h�̕`��
	m_battleSceneData->DrawField();

	//���j�b�g�̕`��
	m_battleSceneData->DrawUnit(false,std::set<const Unit *>{m_battleSceneData->m_operateUnit,m_aimedUnit});

	//�U���Ώۃ��j�b�g�̕`��
	if(m_aimedUnit!=nullptr){
		m_aimedUnit->DrawUnit(Vector2D(),m_battleSceneData->m_fpsMesuring.GetFrame(),false,false);
	}
	
	//�U�������s���郆�j�b�g�̕`��
	m_battleSceneData->m_operateUnit->DrawUnit(Vector2D((float)m_attackMotion.GetX(),(float)m_attackMotion.GetY()),Vector2D(),m_battleSceneData->m_fpsMesuring.GetFrame(),false,false);
	
	//�S���j�b�g��HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();

	//�G�t�F�N�g�̕`��
	if(m_attackMotion.GetFrame()<30){
		//DrawGraph(((int)m_aimedUnit->getPos().x)-50,((int)m_aimedUnit->getPos().y)-50,m_effect[m_attackMotion.GetFrame()],TRUE);
	}

	//�_���[�W�̕`��
	if(m_attackMotion.GetFrame()>damageBeginFrame){
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

void AttackScene::ReturnProcess(){}
