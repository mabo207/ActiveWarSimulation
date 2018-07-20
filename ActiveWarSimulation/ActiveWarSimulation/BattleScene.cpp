#include"BattleScene.h"
#include"SwitchUnitScene.h"

//----------------------BattleScene----------------------
BattleScene::BattleScene(const char *stagename)
	:GameScene(),m_battleSceneData(new BattleSceneData(stagename))
{
	//m_sceneData�̏������A�ŏ��͂ЂƂ܂��ړ���
	m_sceneData=std::shared_ptr<BattleSceneElement>(new SwitchUnitScene(m_battleSceneData));
}

BattleScene::~BattleScene(){}

int BattleScene::Calculate(){
	//�X�V
	m_battleSceneData->m_fpsMesuring.Update();//�^�C�}�[�̍X�V
	int index;
	if(m_sceneData.get()!=nullptr){
		index=m_sceneData->Calculate();
	} else{
		index=1;
	}
	//��ԑJ��
	if(index==BattleSceneElement::SceneKind::END){
		//���̃N���X���Ď�����N���X�ɃQ�[���v���C��ʂ̏I����`����
		return 1;
	}

	return 0;
}

void BattleScene::Draw()const{
	//�`��
	if(m_sceneData.get()!=nullptr){
		m_sceneData->Draw();
	}
}
