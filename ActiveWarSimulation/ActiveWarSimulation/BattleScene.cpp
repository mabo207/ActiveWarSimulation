#include"BattleScene.h"
#include"SwitchUnitScene.h"
#include"TitleScene.h"
#include"input.h"
#include"DxLib.h"
#include"CommonConstParameter.h"

//----------------------BattleScene::BattleSceneFactory----------------------
BattleScene::BattleSceneFactory::BattleSceneFactory(const std::string &stageDirName,const std::string &title,const StageLevel level)
	:SceneFactory()
	,m_stageDirName(stageDirName)
	,m_title(title)
	,m_level(level)
{}

BattleScene::BattleSceneFactory::~BattleSceneFactory(){}

std::shared_ptr<GameScene> BattleScene::BattleSceneFactory::CreateIncompleteScene()const{
	return std::shared_ptr<BattleScene>(new BattleScene(m_stageDirName,m_title,m_level));
}

//----------------------BattleScene----------------------
const int BattleScene::resetInterval=60;

BattleScene::BattleScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:GameScene()
	,m_resetFlag(false)
	,m_resetFrame(0)
	,m_battleSceneData(battleSceneData)
{}

BattleScene::BattleScene(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel)
	:BattleScene(std::shared_ptr<BattleSceneData>(new BattleSceneData(stageDirName,titleName,stageLevel))){}

BattleScene::~BattleScene(){
	//m_battleSceneData�ɂ���V�[���I�����ɍs�������Q�̈ꊇ����������
	m_battleSceneData->RunSceneEndProcess();
}

void BattleScene::InitCompletely(){
	//m_battleSceneData�̓O���t�B�b�N�ƂƂ��ɏ���������ׂ��f�[�^�������̂ŁA�����ŏ����������͍s�킸�ɃR���X�g���N�^�őS�ď���������
}

void BattleScene::Activate(){
	//bgm�Đ�
	m_battleSceneData->m_mapBGM.SetAndPlay(DX_PLAYTYPE_LOOP,TRUE);
	//m_sceneData�̏������A�ŏ��̓��j�b�g�؂�ւ�(m_battleSceneData�̏��������I�������Ԃł��̏����͂������̂�Activate���ōs��)
	m_sceneData=VGetSwitchUnitScene();
}

std::shared_ptr<BattleSceneElement> BattleScene::VGetSwitchUnitScene()const{
	return std::shared_ptr<BattleSceneElement>(new SwitchUnitScene(m_battleSceneData));
}

void BattleScene::ResetGame(){
	m_battleSceneData=std::shared_ptr<BattleSceneData>(new BattleSceneData(m_battleSceneData->m_stageDirName,m_battleSceneData->m_stageTitleName,m_battleSceneData->m_stageLevel));//�o�g���f�[�^��ς���
	m_sceneData=VGetSwitchUnitScene();//�N���X��ς���
	m_battleSceneData->m_mapBGM.SetAndPlay(DX_PLAYTYPE_LOOP,TRUE);//bgm�Đ�
}

int BattleScene::Calculate(){
	//�X�V
	if(!m_resetFlag){
		//�Q�[���������s��
		m_battleSceneData->m_fpsMesuring.Update();//�^�C�}�[�̍X�V
		int index;
		if(m_sceneData.get()!=nullptr){
			index=m_sceneData->Calculate();
		} else{
			index=-2;
		}
		//��ԑJ��
		if(index==BattleSceneElement::SceneKind::END
			|| keyboard_get(KEY_INPUT_DELETE)==60
			)
		{
			//���̃N���X���Ď�����N���X�ɃQ�[���v���C��ʂ̏I����`����
			return -2;
		} else if(index==BattleSceneElement::SceneKind::e_gameReset){
			//���Z�b�g��ʂɈڍs����
			m_resetFlag=true;
		}
	} else{
		//���Z�b�g��ʒ�
		m_resetFrame++;
		if(m_resetFrame==resetInterval/2){
			//���Z�b�g�������s��
			ResetGame();
		} else if(m_resetFrame==resetInterval){
			//���Z�b�g��ʂ��I������
			m_resetFrame=0;
			m_resetFlag=false;
		}
	}

	return 0;
}

void BattleScene::Draw()const{
	//�`��
	if(m_sceneData.get()!=nullptr){
		m_sceneData->Draw();
	}
	//���Z�b�g���͈Ó]����悤�ɂ���
	if(m_resetFlag){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,255*m_resetFrame*(resetInterval-m_resetFrame)/(resetInterval/2)/(resetInterval/2));
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(mode,pal);
	}
}

std::shared_ptr<GameScene> BattleScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//�Q�[���v���C���I��������́A�^�C�g����ʂ�
	const auto titleFactory=std::make_shared<TitleScene::TitleSceneFactory>();
	return CreateFadeOutInSceneCompletely(thisSharedPtr,titleFactory,15,15);
}
