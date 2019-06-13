#include"DxLib.h"
#include"StageSelectUIInStageSelect.h"
#include"input.h"
#include"GeneralPurposeResourceManager.h"

StageSelectUIInStageSelect::StageSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData,const size_t stageNum)
	:BaseUIInStageSelect(controledData)
	,m_stageNum(stageNum)
{}

StageSelectUIInStageSelect::~StageSelectUIInStageSelect(){}

BaseUIInStageSelect::UpdateResult StageSelectUIInStageSelect::Update(){
	//�I���X�e�[�W�̍X�V
	bool indexUpdate=false;
	if(!m_controledData.expired()){
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t beforeIndex=lock->stageIndex;//���ʉ��Đ��̉۔���ɗp����
		if(m_stageNum>0){
			if(keyboard_get(KEY_INPUT_LEFT)==1){
				lock->stageIndex=(lock->stageIndex+m_stageNum-1)%m_stageNum;
			} else if(keyboard_get(KEY_INPUT_RIGHT)==1){
				lock->stageIndex=(lock->stageIndex+1)%m_stageNum;
			}
		}
		indexUpdate=(lock->stageIndex!=beforeIndex);//index���X�V����Ă���Ȃ�true��
	}
	if(indexUpdate){
		//�ύX������Ό��ʉ��Đ�
		PlaySoundMem(GeneralPurposeResourceManager::selectSound,DX_PLAYTYPE_BACK,TRUE);
	} else{
		//�ȉ��̑J�ڂ͑I���X�e�[�W�̕ύX���s���Ă��Ȃ����݂̂ł���
		//�X�e�[�W�̌���
		if(keyboard_get(KEY_INPUT_Z)==1){
			//���艹���o��
			PlaySoundMem(GeneralPurposeResourceManager::decideSound,DX_PLAYTYPE_BACK,TRUE);
			//�J��
			return UpdateResult::e_gotoBattle;
		}
		//�߂�
		if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1){
			//back�����o��
			PlaySoundMem(GeneralPurposeResourceManager::cancelSound,DX_PLAYTYPE_BACK,TRUE);
			//�J��
			return UpdateResult::e_gotoTitle;
		}
	}

	return UpdateResult::e_notTransition;
}

void StageSelectUIInStageSelect::Draw()const{

}

std::shared_ptr<BaseUIInStageSelect> StageSelectUIInStageSelect::GetNextUI(const std::weak_ptr<ControledData> &controledData)const{
	return std::shared_ptr<BaseUIInStageSelect>();
}