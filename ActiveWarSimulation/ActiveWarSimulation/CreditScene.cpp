#include"DxLib.h"
#include"FilePath.h"
#include"GraphicControl.h"
#include"BGMManager.h"
#include"CreditScene.h"
#include"CommonConstParameter.h"
#include"ToolsLib.h"
#include"input.h"

#include"TitleScene.h"

namespace {
	const int professionHeight=60;
	const int nameHeight=45;
	const int pairMergin=20;
	const int creditSpeed=3;
}

//-----------------------CreditScene-----------------
std::shared_ptr<GameScene> CreditScene::CreditSceneFactory::CreateIncompleteScene()const{
	return std::shared_ptr<GameScene>(new CreditScene());
}

CreditScene::CreditScene()
	:m_creditList{CreditPair("���E�v���O�����E�h�b�g�G�Ȃ�",std::vector<std::string>{"�܁[�ڂ�"})
		,CreditPair("�C���X�g�E�L�����N�^�[�f�U�C��",std::vector<std::string>{"���"})
		,CreditPair("�g�p�����Ă����������f�ނ⃉�C�u����",std::vector<std::string>{"DX���C�u���� �l","�U�E�}�b�`���C�J�@�Y �l","�t���[BGM�E���y�f��MusMus �l","Wingless Seraph �l","�������肢 �l(�肢�|�b�v�p R)"})
		,CreditPair("����",std::vector<std::string>{"���H�働�{�b�g�Z�p������̊F�l","���H��f�W�^���n�쓯�D��traP�̊F�l","��w�̗F�l��搶�̕��X"})}
	,m_professionFont(LoadFontDataToHandleEX(FilePath::fontDir+"HugeGothicFont.dft",0))
	,m_nameFont(LoadFontDataToHandleEX(FilePath::fontDir+"LargeThickGothicFont.dft",0))
	,m_backPic(LoadGraphEX(FilePath::graphicDir+"nonfree/titleScene.png"))
	,m_bgm(Resource::BGM::Load("credit.txt"))
	,m_strY(CommonConstParameter::gameResolutionY)
	,m_strHeight(0)
{
	//m_strHeight�̌v�Z
	for(const CreditPair &pair:m_creditList){
		//��E�̕��̍����̉��Z
		m_strHeight+=professionHeight;
		//���O�̕��̍����̉��Z
		m_strHeight+=nameHeight*pair.nameVec.size();
		//�}�[�W���̍����̉��Z
		m_strHeight+=pairMergin;
	}
}

CreditScene::~CreditScene(){
	DeleteFontToHandleEX(m_professionFont);
	DeleteFontToHandleEX(m_nameFont);
	DeleteGraphEX(m_backPic);
	m_bgm.Delete();
	BGMManager::s_instance.value().Stop();//�Ӑ}�I�Ɏ~�߂�
}

void CreditScene::InitCompletely(){
	//���ɂ��邱�Ƃ��Ȃ�
}

void CreditScene::Activate(){
	//bgm�Đ�
	if(BGMManager::s_instance.has_value()){
		BGMManager::s_instance->PlayWithCopy(m_bgm);
	}
}

int CreditScene::Calculate(){
	if(keyboard_get(KEY_INPUT_Z)>0 || mouse_get(MOUSE_INPUT_LEFT)>0){
		//5�{��
		m_strY-=creditSpeed*5;
	} else{
		m_strY-=creditSpeed;
	}
	if(m_strY+m_strHeight<-300){
		return 1;
	}
	return 0;
}

void CreditScene::Draw()const{
	//�w�i�̕`��
	DrawGraph(0,0,m_backPic,TRUE);
	//���킹
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//������̕`��
	int drawY=m_strY;
	for(const CreditPair &pair:m_creditList){
		//��E�̕`��
		DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,drawY,pair.profession.c_str(),GetColor(255,255,255),m_professionFont,false);
		drawY+=professionHeight;
		//���O�̕`��
		for(const std::string &name:pair.nameVec){
			DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,drawY,name.c_str(),GetColor(255,255,255),m_nameFont,false);
			drawY+=nameHeight;
		}
		//�}�[�W���̍����̉��Z
		drawY+=pairMergin;
	}
}

std::shared_ptr<GameScene> CreditScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	const auto titleFactory=std::make_shared<TitleScene::TitleSceneFactory>();
	return CreateFadeOutInSceneCompletely(thisSharedPtr,titleFactory,15,15);
}
