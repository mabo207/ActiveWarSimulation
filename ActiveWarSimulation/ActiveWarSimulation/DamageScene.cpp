#include"DamageScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"FilePath.h"

//-----------------DamageScene----------------
const int DamageScene::motionFrame=20;
const int DamageScene::damageBeginFrame=DamageScene::motionFrame/2;
const int DamageScene::damageEndFrame=DamageScene::damageBeginFrame+30;

DamageScene::DamageScene(SceneKind::Kind sceneKind
	,const std::shared_ptr<BattleSceneData> &data
	,SceneKind::Kind nextSceneKind
	,Unit *aimedUnit
	,const Weapon::AttackInfo &attackInfo)
	:BattleSceneElement(sceneKind)
	,m_battleSceneData(data)
	,m_nextSceneKind(nextSceneKind)
	,m_aimedUnit(aimedUnit)
	,m_frame(0)
	,m_damageMotion({
		PositionControl(0,0,0,-(int)Unit::unitCircleSize,(damageEndFrame-damageBeginFrame)/2,Easing::TYPE_OUT,Easing::FUNCTION_EXPO,9.0)
		,PositionControl(0,0,-(int)Unit::unitCircleSize,-(int)Unit::unitCircleSize,(damageEndFrame-damageBeginFrame)/2,Easing::TYPE_OUT,Easing::FUNCTION_QUAD,9.0)
	})
	,m_attackinfo(attackInfo)
	,m_damageFont(LoadFontDataToHandleEX(FilePath::fontDir+"LargeThickGothicFont.dft",1))
{}

DamageScene::~DamageScene(){
	//�t�H���g�̉��
	DeleteFontToHandleEX(m_damageFont);
}

int DamageScene::thisCalculate(){
	//�A�j���[�V�����X�V
	AnimationUpdate();
	m_frame++;
	//�_���[�W���[�V�����̊J�n
	if(m_frame==damageBeginFrame){
		//�_���[�W�����A�j���[�V�����̏�����
		m_damageMotion.Retry();
		//�_���[�W����
		DamageProcess();
	}
	//�I������
	if(JudgeAnimationEnd()){
		//���j�b�g�؂�ւ���ʂ�
		return m_nextSceneKind;
	}

	return GetSceneKind();
}

void DamageScene::thisDraw()const{
	//�t�B�[���h�̕`��
	m_battleSceneData->DrawField();
	//���j�b�g�̕`��
	DrawUnit();
	//�S���j�b�g��HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();
	//�A�j���[�V�����̕`��
	DrawAnimation();
}

void DamageScene::AnimationUpdate(){
	//�_���[�W�A�j���[�V�����̍X�V
	m_damageMotion.Update();
}

void DamageScene::DrawUnit()const{
	//�_���[�W�Ώۃ��j�b�g�ȊO�̕`��
	m_battleSceneData->DrawUnit(false,std::set<const Unit *>{m_aimedUnit});

	//�_���[�W�Ώۃ��j�b�g�̕`��
	if(m_aimedUnit!=nullptr){
		m_aimedUnit->DrawUnit(Vector2D(),m_battleSceneData->m_fpsMesuring.GetFrame(),false,false,false);
	}
}

void DamageScene::DrawAnimation()const{
	//�f�t�H���g�̓_���[�W�̕`��̂ݍs��
	if(m_frame>damageBeginFrame){
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

bool DamageScene::JudgeAnimationEnd()const{
	//�f�t�H���g�́Am_damageMotion�̊J�n�ƏI���݂̂�����
	return (m_frame>=damageBeginFrame && m_damageMotion.GetEndFlag());
}

void DamageScene::DamageProcess(){
	//�f�t�H���g�́Am_aimedUnit���ǂ��Ȃ���������������
	//���샆�j�b�g���Ώۃ��j�b�g�ւ̍U������
	const int aimedHP=m_aimedUnit->AddHP(-m_attackinfo.damage);
	//���ʉ���炷
	if(m_attackinfo.damage>0){
		//�_���[�W������ΑŌ���
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
	}
}
