#include"DxLib.h"
#include"StageSelectUIInStageSelect.h"
#include"input.h"
#include"GeneralPurposeResourceManager.h"
#include"CommonConstParameter.h"

StageSelectUIInStageSelect::StageSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData,const size_t stageNum,const std::vector<StageInfoInStageSelect> &stageInfoVec)
	:BaseUIInStageSelect(controledData)
	,m_stageNum(stageNum)
	,m_stageInfoVec(stageInfoVec)
{}

StageSelectUIInStageSelect::~StageSelectUIInStageSelect(){}

BaseUIInStageSelect::UpdateResult StageSelectUIInStageSelect::Update(){
	//�I���X�e�[�W�̍X�V
	bool indexUpdate=false;
	bool mouseInStage=false;//�}�E�X���X�e�[�W�������~�̒��ɓ����Ă��邩�ǂ���
	if(!m_controledData.expired()){
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t beforeIndex=lock->stageIndex;//���ʉ��Đ��̉۔���ɗp����
		if(m_stageNum>0){
			//�\���L�[�ł̐؂�ւ�
			if(keyboard_get(KEY_INPUT_LEFT)==1){
				lock->stageIndex=(lock->stageIndex+m_stageNum-1)%m_stageNum;
			} else if(keyboard_get(KEY_INPUT_RIGHT)==1){
				lock->stageIndex=(lock->stageIndex+1)%m_stageNum;
			} else{
				//�}�E�X�ł̐؂�ւ�
				const Vector2D mousePos=GetMousePointVector2D();
				const float circleSize=30.0f;//�����蔻��̉~�̑傫��
				for(size_t i=0;i<m_stageNum;i++){
					if((m_stageInfoVec[i].m_pos-mousePos).sqSize()<=circleSize*circleSize){
						lock->stageIndex=i;
						mouseInStage=true;
						break;
					}
				}
			}
		}
		indexUpdate=(lock->stageIndex!=beforeIndex);//index���X�V����Ă���Ȃ�true��
	}
	//�I���ȊO�̓��͏���
	if(indexUpdate){
		//�ύX������Ό��ʉ��Đ�
		PlaySoundMem(GeneralPurposeResourceManager::selectSound,DX_PLAYTYPE_BACK,TRUE);
	} else{
		//�ȉ��̑J�ڂ͑I���X�e�[�W�̕ύX���s���Ă��Ȃ����݂̂ł���
		//�X�e�[�W�̌���
		if(keyboard_get(KEY_INPUT_Z)==1
			|| (mouse_get(MOUSE_INPUT_LEFT)==1 && mouseInStage))
		{
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
	if(!m_controledData.expired()){
		//�I�����Ă���X�e�[�W�̕\���Əꏊ�̋���
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		if(lock->stageIndex<m_stageInfoVec.size()){
			//�ꏊ�̋���
			const Vector2D pos=m_stageInfoVec[lock->stageIndex].m_pos;
			DrawCircleAA(pos.x,pos.y,30,10,GetColor(255,255,255),TRUE);
			//�X�e�[�W�G�̕`��
			int picX,picY;
			if((int)pos.x<CommonConstParameter::gameResolutionX/2){
				picX=CommonConstParameter::gameResolutionX*3/4;
			} else{
				picX=CommonConstParameter::gameResolutionX/4;
			}
			if((int)pos.y<CommonConstParameter::gameResolutionY/2){
				picY=CommonConstParameter::gameResolutionY*3/4;
			} else{
				picY=CommonConstParameter::gameResolutionY/4;
			}
			int picDX,picDY;
			GetGraphSize(m_stageInfoVec[lock->stageIndex].m_mapPic,&picDX,&picDY);
			DrawGraph(picX-picDX/2,picY-picDY/2,m_stageInfoVec[lock->stageIndex].m_mapPic,TRUE);
			//�X�e�[�W���̕`��
			
		}
	}
}

std::shared_ptr<BaseUIInStageSelect> StageSelectUIInStageSelect::GetNextUI(const std::weak_ptr<ControledData> &controledData)const{
	return std::shared_ptr<BaseUIInStageSelect>();
}