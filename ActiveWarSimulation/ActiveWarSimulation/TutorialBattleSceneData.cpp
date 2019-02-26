#include"DxLib.h"
#include"TutorialBattleSceneData.h"
#include"Circle.h"
#include"CommonConstParameter.h"
#include"ToolsLib.h"
#include"GraphicControl.h"

//----------------TutorialBattleSceneData::MoveTutorial----------------------
TutorialBattleSceneData::MoveTutorial::MoveTutorial(std::shared_ptr<Shape> moveTutorialArea)
	:TutorialBase(TutorialKind::e_move)
	,m_moveTutorialArea(moveTutorialArea)
{}

TutorialBattleSceneData::MoveTutorial::~MoveTutorial(){}

void TutorialBattleSceneData::MoveTutorial::DrawSupplement(int font)const{
	int width=650,height=250;
	const int x=CommonConstParameter::gameResolutionX-width,y=0;
	//���o�b�N�𔼓����ŕ`��
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//������̕`��
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"�ړ�\n"
		"�@�}�E�X�@�@�@�F���N���b�N�ł��̏ꏊ�Ɍ������Ĉړ�\n"
		"�@�Q�[���p�b�h�F���W���C�X�e�B�b�N or �\���L�[\n"
		"�@�L�[�{�[�h�@�F�㉺���E�L�[\n\n"
		"�ړ��̂�蒼��\n"
		"�@�}�E�X�@�@�@�F�E�N���b�N\n"
		"�@�Q�[���p�b�h�F�{�^���R\n"
		"�@�L�[�{�[�h�@�FX�L�["
		,GetColor(255,255,255),font,2);
}

//----------------TutorialBattleSceneData::AttackTutorial----------------------
TutorialBattleSceneData::AttackTutorial::AttackTutorial(const Unit *targetUnit)
	:TutorialBase(TutorialKind::e_normalAttack)
	,m_targetUnit(targetUnit)
{}

TutorialBattleSceneData::AttackTutorial::~AttackTutorial(){}

void TutorialBattleSceneData::AttackTutorial::DrawSupplement(int font)const{
	int width=650,height=250;
	const int x=CommonConstParameter::gameResolutionX-width,y=0;
	//���o�b�N�𔼓����ŕ`��
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//������̕`��
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"�U��\n"
		"�@�}�E�X�@�@�@�F�U�����������j�b�g�����N���b�N\n"
		"�@�Q�[���p�b�h�F�{�^���S\n"
		"�@�L�[�{�[�h�@�FZ�L�[\n\n"
		"�U���Ώۂ̕ύX\n"
		"�@�}�E�X�@�@�@�F�U�����������j�b�g�Ƀ}�E�X�����킹��\n"
		"�@�Q�[���p�b�h�F�{�^���V�E�{�^���W\n"
		"�@�L�[�{�[�h�@�FA�L�[�ES�L�["
		,GetColor(255,255,255),font,2);
}

//----------------TutorialBattleSceneData::WaitTutorial----------------------
TutorialBattleSceneData::WaitTutorial::WaitTutorial()
	:TutorialBase(TutorialKind::e_wait)
{}

TutorialBattleSceneData::WaitTutorial::~WaitTutorial(){}

void TutorialBattleSceneData::WaitTutorial::DrawSupplement(int font)const{
	int width=650,height=120;
	const int x=CommonConstParameter::gameResolutionX-width,y=0;
	//���o�b�N�𔼓����ŕ`��
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//������̕`��
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"�ҋ@\n"
		"�@�}�E�X�@�@�@�F�E����NEXT�{�^�������N���b�N\n"
		"�@�Q�[���p�b�h�F�{�^���Q\n"
		"�@�L�[�{�[�h�@�FV�L�["
		,GetColor(255,255,255),font,2);
}

//----------------TutorialBattleSceneData-----------------------
TutorialBattleSceneData::TutorialBattleSceneData()
	:BattleSceneData("tutorial",PlayMode::e_tutorial)
	,m_tutorialFont(CreateFontToHandleEX("���C���I",24,1,-1))
{
	//�`���[�g���A���f�[�^�̓ǂݍ���

	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new MoveTutorial(std::shared_ptr<Shape>(new Circle(Vector2D(395.0f,425.0f),45.0f,Shape::Fix::e_ignore)))));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new AttackTutorial(m_unitList[1])));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new WaitTutorial()));
}

TutorialBattleSceneData::~TutorialBattleSceneData()
{
}
