#include"DemoScene.h"
#include"DemoSwitchUnitScene.h"
#include"input.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"

//--------------------DemoScene::DemoSceneFactory-------------------
DemoScene::DemoSceneFactory::DemoSceneFactory()
	:SceneFactory()
{}

DemoScene::DemoSceneFactory::~DemoSceneFactory(){}

std::shared_ptr<GameScene> DemoScene::DemoSceneFactory::CreateScene()const{
	return std::shared_ptr<GameScene>(new DemoScene());
}

//--------------------DemoScene-------------------
DemoScene::DemoScene()
	:BattleScene("demo","�f���X�e�[�W",3)
	,m_frame(0)
	,m_font(CreateFontToHandleEX("���C���I",36,5,DX_FONTTYPE_ANTIALIASING_EDGE,-1,3))
{
	//m_sceneData�̏���������蒼���ABattleScene�̃R���X�g���N�^���ł�DemoScene�̊֐��͌Ăяo���Ȃ�
	ResetGame();
}

DemoScene::~DemoScene(){
	DeleteFontToHandleEX(m_font);
}

std::shared_ptr<BattleSceneElement> DemoScene::VGetSwitchUnitScene()const{
	return std::shared_ptr<BattleSceneElement>(new DemoSwitchUnitScene(m_battleSceneData));
}

int DemoScene::Calculate(){
	//�]����Calculate()������
	int index=BattleScene::Calculate();
	m_frame++;
	//�N���b�N�܂��͌���L�[�̓��͂����m���A���m������f�����I��点��
	if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
		return -2;
	}
	return index;
}

void DemoScene::Draw()const{
	//�]����Draw
	BattleScene::Draw();
	//���ӏ���
	if((m_frame/120)%2==0){
		DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/2,"Z�L�[�A4�{�^���A����{�^���܂��̓}�E�X�����N���b�N����ƃ^�C�g����ʂɖ߂�܂��B",GetColor(255,255,255),m_font,true,GetColor(0,0,0));
	}
}
