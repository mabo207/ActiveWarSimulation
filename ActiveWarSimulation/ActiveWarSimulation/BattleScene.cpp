#include"BattleScene.h"
#include"SwitchUnitScene.h"
#include"TitleScene.h"
#include"input.h"
#include"DxLib.h"
#include"CommonConstParameter.h"

//----------------------BattleScene::BattleSceneFactory----------------------
BattleScene::BattleSceneFactory::BattleSceneFactory(const std::string &stagename)
	:MainSceneFactory()
	,m_stagename(stagename)
{}

BattleScene::BattleSceneFactory::~BattleSceneFactory(){}

std::shared_ptr<MainControledGameScene> BattleScene::BattleSceneFactory::CreateScene()const{
	return std::make_shared<BattleScene>(m_stagename.c_str());
}

//----------------------BattleScene----------------------
const int BattleScene::resetInterval=60;

BattleScene::BattleScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MainControledGameScene()
	,m_resetFlag(false)
	,m_resetFrame(0)
	,m_battleSceneData(battleSceneData)
{
	//bgm�Đ�
	PlaySoundMem(m_battleSceneData->m_mapBGM,DX_PLAYTYPE_LOOP,TRUE);
	//m_sceneData�̏������A�ŏ��͂ЂƂ܂��ړ���
	m_sceneData=VGetSwitchUnitScene();
}

BattleScene::BattleScene(const char *stagename)
	:BattleScene(std::shared_ptr<BattleSceneData>(new BattleSceneData(stagename))){}

BattleScene::~BattleScene(){}

std::shared_ptr<BattleSceneElement> BattleScene::VGetSwitchUnitScene()const{
	return std::shared_ptr<BattleSceneElement>(new SwitchUnitScene(m_battleSceneData));
}

void BattleScene::ResetGame(){
	m_battleSceneData=std::shared_ptr<BattleSceneData>(new BattleSceneData(m_battleSceneData->m_stageName));//�o�g���f�[�^��ς���
	m_sceneData=VGetSwitchUnitScene();//�N���X��ς���
	PlaySoundMem(m_battleSceneData->m_mapBGM,DX_PLAYTYPE_LOOP,TRUE);//bgm�Đ�
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

std::shared_ptr<MainControledGameScene> BattleScene::VGetNextMainControledScene()const{
	//�Q�[���v���C���I��������́A�^�C�g����ʂ�
	return TitleScene::TitleSceneFactory().CreateScene();
}
