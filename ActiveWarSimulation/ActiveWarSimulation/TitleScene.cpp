#define _USE_MATH_DEFINES
#include"TitleScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"MyPolygon.h"
#include"input.h"
#include<cmath>
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"
#include"FilePath.h"
#include"BGMManager.h"

#include"StageSelectScene.h"
#include"BattleScene.h"
#include"DemoScene.h"
#include"TutorialScene.h"
#include"FadeInScene.h"
#include"FadeOutScene.h"

//-------------------TitleScene::SelectItem-------------------
std::string TitleScene::SelectItem::GetString(const Kind kind){
	switch(kind){
	case(e_stageSelect):
		return "STAGE SELECT";
	case(e_tutorial):
		return "TUTORIAL";
	case(e_tutorial_2):
		return "TUTORIAL2";
	case(e_gameFinish):
		return "EXIT GAME";
	}
	return "";
}

//-------------------TitleScene::TitleSceneFactory-------------------
TitleScene::TitleSceneFactory::TitleSceneFactory()
	:SceneFactory()
{}

TitleScene::TitleSceneFactory::~TitleSceneFactory(){}

std::shared_ptr<GameScene> TitleScene::TitleSceneFactory::CreateIncompleteScene()const{
	return std::shared_ptr<GameScene>(new TitleScene());
}

//-------------------TitleScene-------------------
std::shared_ptr<Shape> TitleScene::MakeHexagon(const Vector2D center,const float size)const{
	const size_t vecNum=6;
	Vector2D vec[vecNum];
	for(size_t i=0;i<vecNum;i++){
		vec[i]=Vector2D(size,0.0f).turn((float)(M_PI*2*i/vecNum+M_PI/6));
	}
	const Vector2D begin=center+vec[0];
	std::vector<Vector2D> point(vecNum-1,Vector2D());
	for(size_t i=0;i+1<vecNum;i++){
		point[i]=center+vec[i+1];
	}
	return std::shared_ptr<Shape>(new MyPolygon(begin,point,Shape::Fix::e_static));
}

const Vector2D TitleScene::strPos[TitleScene::SelectItem::COUNTER]={
	Vector2D(600.0f,580.0f)
	,Vector2D(600.0f,930.0f)
	,Vector2D(1320.0f,580.0f)
	,Vector2D(1320.0f,930.0f)
};

TitleScene::TitleScene()
	:GameScene()
	,m_backPic(LoadGraphEX(FilePath::graphicDir+"nonfree/titleScene.png"))
	,m_itemPic(LoadGraphEX(FilePath::graphicDir+"nonfree/titleItem.png"))
	,m_itemFont(CreateFontToHandleEX("���C���I",24,2,-1))
	,m_bgm(Resource::BGM::Load("title.txt"))
	,m_aimchangeSound(LoadSoundMem((FilePath::effectSoundDir+"nonfree/aimchange.ogg").c_str()))
	,m_mousePosJustBefore(GetMousePointVector2D())
	,m_selectItem(SelectItem::e_stageSelect)
	,m_frame(0)
	,m_selectLocked(true)
	,m_gotoDemo(false)
{
	m_itemAlpha=Easing(0,60,Easing::TYPE_IN,Easing::FUNCTION_LINER,0.0);
	m_itemAlpha.SetTarget(255,true);
	//�����蔻��}�`���̗p��
	for(size_t i=0;i<SelectItem::COUNTER;i++){
		m_hitJudgeShapeVec[i]=MakeHexagon(strPos[i],80.0f);
		m_itemPosVec[i]=PositionControl(960,750,60,Easing::TYPE_OUT,Easing::FUNCTION_QUAD,2.0);
		m_itemPosVec[i].SetTarget((int)strPos[i].x,(int)strPos[i].y,true);
	}
}

TitleScene::~TitleScene(){
	//�O���t�B�b�N���
	DeleteGraphEX(m_backPic);
	DeleteGraphEX(m_itemPic);
	DeleteFontToHandleEX(m_itemFont);
	//�T�E���h���
	m_bgm.Delete();
	DeleteSoundMem(m_aimchangeSound);
}

void TitleScene::InitCompletely(){
	//���ɂ��鎖�͂Ȃ�
}

void TitleScene::Activate(){
	//bgm�Đ�
	if(BGMManager::s_instance.has_value()){
		BGMManager::s_instance->PlayWithCopy(m_bgm);
	}
	//m_bgm.SetAndPlay(DX_PLAYTYPE_LOOP,TRUE);
}

int TitleScene::thisCalculate(){
	//�t���[�����̍X�V
	m_frame++;

	//����ȓ��͏��̎擾
	const Vector2D mousePos=GetMousePointVector2D();//�}�E�X�ʒu
	const Vector2D stickInput=analogjoypad_get(DX_INPUT_PAD1);//�A�i���O�X�e�B�b�N�̌X��

	if(m_selectLocked){
		//�N���b�N����܂ő҂ꍇ
		if(keyboard_get(KEY_INPUT_Z)==1	|| mouse_get(MOUSE_INPUT_LEFT)==1){
			m_selectLocked=false;
		} else if(m_frame>1800){
			//30�b���u���Ă���ƁA�f����ʂɐi��
			m_gotoDemo=true;
		}
	}else{
		//���ڑI�����ł��鎞
		//�I�����ڂ̕`�擧���x���X�V
		m_itemAlpha.Update();
		//�`��ʒu�̍X�V
		for(size_t i=0;i<SelectItem::COUNTER;i++){
			m_itemPosVec[i].Update();
		}
		//�I�����ڂ̍X�V
		const size_t beforeSelectItem=m_selectItem;//���ʉ���炷���̔���ɗp����
		if((m_mousePosJustBefore-mousePos).sqSize()>4.0f){
			//�}�E�X���傫����������A�}�E�X�̈ʒu�����ƂɍX�V
			for(size_t i=0;i<SelectItem::COUNTER;i++){
				if(m_hitJudgeShapeVec[i]->VJudgePointInsideShape(mousePos)){
					//�}�`�S�Ăɓ_�̓���������s���A�����ɂ������}�`�ɑΉ����鍀�ڂɕύX����B
					//�ǂ̐}�`�ɂ��Ή����Ȃ�������ύX�͂��Ȃ�
					m_selectItem=static_cast<SelectItem::Kind>(i);//�ύX
					break;
				}
			}
		} else{
			//�����łȂ��ꍇ�́A�L�[���͂ōs��
			if(keyboard_get(KEY_INPUT_UP)==1 || keyboard_get(KEY_INPUT_DOWN)==1){
				switch(m_selectItem){
				case(SelectItem::e_stageSelect):
					m_selectItem=SelectItem::e_tutorial;
					break;
				case(SelectItem::e_tutorial):
					m_selectItem=SelectItem::e_stageSelect;
					break;
				case(SelectItem::e_tutorial_2):
					m_selectItem=SelectItem::e_gameFinish;
					break;
				case(SelectItem::e_gameFinish):
					m_selectItem=SelectItem::e_tutorial_2;
					break;
				}
			} else if(keyboard_get(KEY_INPUT_LEFT)==1 || keyboard_get(KEY_INPUT_RIGHT)==1){
				switch(m_selectItem){
				case(SelectItem::e_stageSelect):
					m_selectItem=SelectItem::e_tutorial_2;
					break;
				case(SelectItem::e_tutorial):
					m_selectItem=SelectItem::e_gameFinish;
					break;
				case(SelectItem::e_tutorial_2):
					m_selectItem=SelectItem::e_stageSelect;
					break;
				case(SelectItem::e_gameFinish):
					m_selectItem=SelectItem::e_tutorial;
					break;
				}
			}
		}
		if(beforeSelectItem!=m_selectItem){
			//�I�����ڂ̕ύX���N���Ă���Ȃ�Ό��ʉ��Đ�
			PlaySoundMem(m_aimchangeSound,DX_PLAYTYPE_BACK,TRUE);
		}

		//�J�ړ��͏���
		if(keyboard_get(KEY_INPUT_Z)==1
			|| (mouse_get(MOUSE_INPUT_LEFT)==1 && m_hitJudgeShapeVec[m_selectItem]->VJudgePointInsideShape(mousePos))
			)
		{
			//����L�[���͂��A�{�^���̏�ō��N���b�N
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//���ʉ��Đ�
			return m_selectItem;
		}
	}

	//�}�E�X�ʒu�̍X�V
	m_mousePosJustBefore=mousePos;

	return SelectItem::COUNTER;//SelectItem::COUNTER�͌���ێ�
}

int TitleScene::Calculate(){
	int index=thisCalculate();//�X�V����
	//�J�ڏ���
	switch(index){
	case(SelectItem::e_gameFinish):
		//�O�̃N���X�ɂ��ǂ�B
		return 1;
	case(SelectItem::e_stageSelect):
		//�X�e�[�W�Z���N�g��ʂ�
		//m_nextScene=std::shared_ptr<GameScene>(new StageSelectScene(m_sharedData));
		//break;
		return 1;
	case(SelectItem::e_tutorial):
		//�`���[�g���A����ʂ�
		return 1;
		break;
	case(SelectItem::e_tutorial_2):
		//�`���[�g���A�����2��
		return 1;
		break;
	case(SelectItem::COUNTER):
		//��{�I�Ɍ���ێ�
		if(m_gotoDemo){
			return 1;//m_gotoDemo��true�ɂȂ����ꍇ��������ȑJ��
		}
		break;
	default:
		break;
	}
	return 0;
}

void TitleScene::Draw()const{
	//�w�i�̕`��
	DrawGraph(0,0,m_backPic,TRUE);
	//�o�[�W�������
	const std::string VERSION_STRING="- ver 1.0 -";
	const int verX=CommonConstParameter::gameResolutionX-GetDrawStringWidthToHandle(VERSION_STRING.c_str(),VERSION_STRING.size(),m_itemFont);
	const int verY=CommonConstParameter::gameResolutionY-GetFontSizeToHandle(m_itemFont);
	DrawStringToHandle(verX,verY,VERSION_STRING.c_str(),GetColor(0,0,0),m_itemFont);
	if(m_selectLocked){
		//�����̕`��
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,120+(int)(135*std::cos(M_PI*2*m_frame/120)));
		DrawStringCenterBaseToHandle(960,800,"CLICK OR PRESS TO START",GetColor(255,255,255),GeneralPurposeResource::popLargeFont,true,GetColor(0,0,0));
		SetDrawBlendMode(mode,pal);
	}else{
		//���ڂ̕`��
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,m_itemAlpha.GetX());
		for(size_t i=0;i<SelectItem::COUNTER;i++){
			const unsigned int fontColor=GetColor(0,0,0);
			double rotate=0.0;
			int strDy=0;
			double exRate=1.0;
			if(i==m_selectItem){
				rotate=M_PI*2*m_frame/120;
				exRate=1.1;
				strDy=(int)(std::cos(rotate)*5.0);
			}
			DrawRotaGraph(m_itemPosVec[i].GetX(),m_itemPosVec[i].GetY(),exRate,rotate,m_itemPic,TRUE);
			DrawStringCenterBaseToHandle(m_itemPosVec[i].GetX(),m_itemPosVec[i].GetY()+strDy,SelectItem::GetString(static_cast<SelectItem::Kind>(i)).c_str(),fontColor,m_itemFont,true);
		}
		SetDrawBlendMode(mode,pal);
	}
}

std::shared_ptr<GameScene> TitleScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	if(m_gotoDemo){
		const auto demo=std::make_shared<DemoScene::DemoSceneFactory>();
		return CreateFadeOutInSceneCompletely(thisSharedPtr,demo,15,15);
	} else if(m_selectItem==SelectItem::e_stageSelect){
		const auto stageselect=std::make_shared<StageSelectScene::StageSelectSceneFactory>();
		return CreateFadeOutInSceneCompletely(thisSharedPtr,stageselect,15,15);
	} else if(m_selectItem==SelectItem::e_tutorial){
		const auto tutorial=std::make_shared<TutorialScene::TutorialSceneFactory>("tutorial");
		return CreateFadeOutInSceneCompletely(thisSharedPtr,tutorial,15,15);
	} else if(m_selectItem==SelectItem::e_tutorial_2){
		const auto tutorial=std::make_shared<TutorialScene::TutorialSceneFactory>("tutorial_2");
		return CreateFadeOutInSceneCompletely(thisSharedPtr,tutorial,15,15);
	}
	return std::shared_ptr<GameScene>();
}
