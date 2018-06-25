#include"BattleSceneElement.h"

//---------------BattleSceneElement-----------------
BattleSceneElement::BattleSceneElement(SceneKind::Kind sceneKind)
	:m_nextScene(nullptr),m_sceneKind(sceneKind){}

BattleSceneElement::~BattleSceneElement(){}

BattleSceneElement::SceneKind::Kind BattleSceneElement::GetTailSceneKind()const{
	//�����ɂ����ʃN���X��m_sceneKind�𓾂�
	if(m_nextScene.get()!=nullptr){
		return m_nextScene->GetTailSceneKind();
	} else{
		return GetSceneKind();
	}
}

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
	if(index==SceneKind::END){
		//�Q�[���v���C���I��鎞
		m_nextScene.reset();
	} else if(m_nextScene.get()==nullptr){
		//���̃N���X���ŏ��������Ă����ꍇ
		if(index!=m_sceneKind){
			//���̃N���X���g�̏�������m_nextScene�֐i�ޏꍇ
			//���̏���������m_sceneKind�������ł����ʂɐi�߂Ȃ����ɂȂ邪�c���Ȃ���
			ret=UpdateNextScene(index);
		} else if(index==0){
			//���O��ʂɖ߂�ꍇ
			//Calaulate()��0��Ԃ��Ή��������Ƃ����O�ɖ߂�
		}
	} else{
		//m_nextScene�ȍ~�ŏ��������Ă����ꍇ
		if(index!=m_nextScene->GetTailSceneKind()){
			if(index==0 || index==m_sceneKind){
				//���̏�ʂɖ߂��Ă����ꍇ
				ReturnProcess();
				m_nextScene.reset();
				ret=m_sceneKind;//�Ԃ�l�͂��̃N���X���g�̎�ޒl�ɂ���
			} else{
				//���̃N���X�̏�������index�ɓ������ʂ܂ň�C�ɖ߂�ꍇ
				ReturnProcess();
				m_nextScene.reset();
			}
		}
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
