#include"DxLib.h"
#include"UnitFactory.h"
#include"EditActionSettings.h"
#include"StageEditor.h"
#include"Circle.h"

//----------------UnitFactory::UnitFactoryButton-------------------
UnitFactory::UnitFactoryButton::UnitFactoryButton(Vector2D point,Vector2D vec,Unit::Profession::Kind profession)
	:ShapeFactoryButton(point,vec)
	,m_profession(profession)
{}

void UnitFactory::UnitFactoryButton::ButtonDraw(int font,int fillFlag)const{
	//�����������ă{�^���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//���g(���傢���߂�����)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//�g���̕`��
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//�O�g
	//����̕`��
	DrawStringCenterBaseToHandle((int)(m_point+m_vec/2).x,(int)(m_point+m_vec/2).y,Unit::Profession::GetName(m_profession).c_str(),GetColor(0,0,0),font,true);
}

void UnitFactory::UnitFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new UnitFactory(m_point,m_vec,GetColor(0,255,255),m_profession));
	//�I�u�W�F�N�g��I�����Ă���ꍇ�͍X�V����
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject=settings.m_pShapeFactory->CreateObject(settings.m_pBattleObject->getPos());
	}
}

//----------------UnitFactory-----------------
UnitFactory::UnitFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightColor,const Unit::Profession::Kind profession)
	:ShapeFactory(buttonPos,buttonSize,lightColor)
	,m_profession(profession)
{}

UnitFactory::~UnitFactory(){}

std::shared_ptr<BattleObject> UnitFactory::CreateObject(Vector2D point)const{
	//�ЂƂ܂��v���C���[���ELv1�E�ˌ�AI�E��{�����̃��j�b�g���쐬����
	return std::shared_ptr<BattleObject>(Unit::CreateMobUnit("mob",m_profession,1,point,Unit::Team::e_player,Unit::AIType::e_assult,0,{}));
}
