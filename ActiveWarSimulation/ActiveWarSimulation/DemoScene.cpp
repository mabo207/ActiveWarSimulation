#include"DemoScene.h"
#include"DemoSwitchUnitScene.h"
#include"input.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"

#include"TitleScene.h"

//--------------------DemoScene::DemoSceneFactory-------------------
DemoScene::DemoSceneFactory::DemoSceneFactory()
	:SceneFactory()
{}

DemoScene::DemoSceneFactory::~DemoSceneFactory(){}

std::shared_ptr<GameScene> DemoScene::DemoSceneFactory::CreateIncompleteScene()const{
	return std::shared_ptr<GameScene>(new DemoScene());
}

//--------------------DemoScene-------------------
DemoScene::DemoScene()
	:BattleScene("demo","�f���X�e�[�W",StageLevel::e_easy)
	,m_frame(0)
	,m_font(CreateFontToHandleEX("���C���I",36,5,DX_FONTTYPE_ANTIALIASING_EDGE,-1,3))
{}

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

std::shared_ptr<GameScene> DemoScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//�Q�[���v���C���I��������́A�X�e�[�W�Z���N�g��ʂ�
	const auto titleFactory=std::make_shared<TitleScene::TitleSceneFactory>();
	return CreateFadeOutInSceneCompletely(thisSharedPtr,titleFactory,15,15);
}
