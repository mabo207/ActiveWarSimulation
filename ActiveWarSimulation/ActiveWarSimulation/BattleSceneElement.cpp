#include"BattleSceneElement.h"

//---------------BattleSceneElement-----------------
BattleSceneElement::BattleSceneElement(SceneKind::Kind sceneKind)
	:m_nextScene(nullptr),m_sceneKind(sceneKind){}

BattleSceneElement::~BattleSceneElement(){}

int BattleSceneElement::Calculate(){
	//�X�V����
	int index;
	if(m_nextScene.get()==nullptr){
		index=thisCalculate();
	} else{
		index=m_nextScene->Calculate();
	}
	//��ʑJ��
	int ret=index;
	if(index==0){
		//���̏�ʂɖ߂��Ă����ꍇ
		ReturnProcess();
		m_nextScene.reset();
	} else if(index==SceneKind::END){
		//�Q�[���v���C���I��鎞
		m_nextScene.reset();
	} else if(index!=m_sceneKind && m_nextScene.get()==nullptr){
		//���̃N���X���g�̏�������m_nextScene�֐i�ޏꍇ
		//���̏���������m_sceneKind�������ł����ʂɐi�߂Ȃ����ɂȂ邪�c���Ȃ���
		ret=UpdateNextScene(index);
	}
	return ret;
}

void BattleSceneElement::Draw()const{
	//�`�揈��
	if(m_nextScene.get()==nullptr){
		thisDraw();
	} else{
		m_nextScene->Draw();
	}

}
