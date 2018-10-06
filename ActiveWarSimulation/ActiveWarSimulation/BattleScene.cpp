#include"BattleScene.h"
#include"SwitchUnitScene.h"
#include"TitleScene.h"
#include"input.h"
#include"DxLib.h"

//----------------------BattleScene----------------------
const int BattleScene::resetInterval=60;

BattleScene::BattleScene(const char *stagename)
	:MainControledGameScene()
	,m_resetFlag(false)
	,m_resetFlame(0)
	,m_battleSceneData(new BattleSceneData(stagename))
{
	//bgm�Đ�
	PlaySoundMem(m_battleSceneData->m_mapBGM,DX_PLAYTYPE_LOOP,TRUE);
	//m_sceneData�̏������A�ŏ��͂ЂƂ܂��ړ���
	m_sceneData=VGetSwitchUnitScene();
}

BattleScene::~BattleScene(){}

std::shared_ptr<BattleSceneElement> BattleScene::VGetSwitchUnitScene()const{
	return std::shared_ptr<BattleSceneElement>(new SwitchUnitScene(m_battleSceneData));
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
		m_resetFlame++;
		if(m_resetFlame==resetInterval/2){
			//���Z�b�g�������s��
			m_battleSceneData=std::shared_ptr<BattleSceneData>(new BattleSceneData(m_battleSceneData->m_stageName));//�o�g���f�[�^��ς���
			m_sceneData=VGetSwitchUnitScene();//�N���X��ς���
		} else if(m_resetFlame==resetInterval){
			//���Z�b�g��ʂ��I������
			m_resetFlame=0;
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
}

std::shared_ptr<MainControledGameScene> BattleScene::VGetNextMainControledScene()const{
	//�Q�[���v���C���I��������́A�^�C�g����ʂ�
	return std::shared_ptr<MainControledGameScene>(new TitleScene());
}
