#include"DxLib.h"
#include"StageSelectUIInStageSelect.h"
#include"input.h"
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"

StageSelectUIInStageSelect::StageSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData
	,const std::vector<StageInfoInStageSelect> &stageInfoVec
	,int stageNameFont
	,int explainFont
)
	:BaseUIInStageSelect(controledData)
	,m_beforeFrameMousePos(GetMousePointVector2D())
	,m_drawStageInfo(true)
	,m_stageInfoVec(stageInfoVec)
	,m_stageNameFont(stageNameFont)
	,m_explainFont(explainFont)
{}

StageSelectUIInStageSelect::~StageSelectUIInStageSelect(){}

BaseUIInStageSelect::UpdateResult StageSelectUIInStageSelect::Update(){
	//�I���X�e�[�W�̍X�V
	bool indexUpdate=false;
	bool mouseInStage=false;
	const Vector2D mousePos=GetMousePointVector2D();//���̃t���[���̃}�E�X�̈ʒu
	const bool mouseMoveFlag=((mousePos-m_beforeFrameMousePos).sqSize()>=4.0f);//���̃t���[���Ń}�E�X�̈ړ����s������
	if(!m_controledData.expired()){
		const size_t stageNum=m_stageInfoVec.size();
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t beforeIndex=lock->stageIndex;//���ʉ��Đ��̉۔���ɗp����
		//�X�V����
		if(stageNum>0){
			//�\���L�[�ł̐؂�ւ�
			if(keyboard_get(KEY_INPUT_LEFT)==1){
				lock->stageIndex=(lock->stageIndex+stageNum-1)%stageNum;
				//m_drawStageInfo�̍X�V
				m_drawStageInfo=true;
			} else if(keyboard_get(KEY_INPUT_RIGHT)==1){
				lock->stageIndex=(lock->stageIndex+1)%stageNum;
				//m_drawStageInfo�̍X�V
				m_drawStageInfo=true;
			} else{
				//�}�E�X�ł̐؂�ւ�
				const float circleSize=30.0f;//�����蔻��̉~�̑傫��
				for(size_t i=0;i<stageNum;i++){
					if((m_stageInfoVec[i].m_pos-mousePos).sqSize()<=circleSize*circleSize){
						if(mouseMoveFlag){
							//�}�E�X����苗���ȏ㓮�������ꍇ�̂ݍX�V
							lock->stageIndex=i;
						}
						mouseInStage=(lock->stageIndex==i);//�}�E�X���w���Ă���X�e�[�W�ƌ��ݑI�����Ă���X�e�[�W����v���Ă��邩
						break;
					}
				}
				//m_drawStageInfo�̍X�V
				if(mouseInStage){
					//�}�E�X���X�e�[�W�̒��ɂ���΃X�e�[�W�I�����Ă��邱�Ƃɂ���
					m_drawStageInfo=true;
				} else if(mouseMoveFlag){
					//�}�E�X�𓮂������̂Ƀ}�E�X���X�e�[�W�I�������Ă��Ȃ��ꍇ�̓X�e�[�W�������I�΂�Ă��Ȃ���Ԃɂ���
					m_drawStageInfo=false;
				}
			}
		}
		//�X�V���ʂɂ��ċL�^
		indexUpdate=(lock->stageIndex!=beforeIndex);//index���X�V����Ă���Ȃ�true��
	}
	//�I���ȊO�̓��͏���
	if(indexUpdate){
		//�ύX������Ό��ʉ��Đ�
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);
	} else{
		//�ȉ��̑J�ڂ͑I���X�e�[�W�̕ύX���s���Ă��Ȃ����݂̂ł���
		//�X�e�[�W�̌���
		if(keyboard_get(KEY_INPUT_Z)==1
			|| (mouse_get(MOUSE_INPUT_LEFT)==1 && mouseInStage))
		{
			//���艹���o��
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
			//�J��
			return UpdateResult::e_gotoLevelSelect;
		}
		//�߂�
		if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1){
			//back�����o��
			PlaySoundMem(GeneralPurposeResource::cancelSound,DX_PLAYTYPE_BACK,TRUE);
			//�J��
			return UpdateResult::e_gotoTitle;
		}
	}
	//m_beforeFrameMousePos�̍X�V
	m_beforeFrameMousePos=mousePos;

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
			//�X�e�[�W���̕`��
			if(m_drawStageInfo){
				int picX,picY;
				if((int)pos.x<CommonConstParameter::gameResolutionX/2){
					picX=CommonConstParameter::gameResolutionX*5/8;
				} else{
					picX=CommonConstParameter::gameResolutionX/8;
				}
				if((int)pos.y<CommonConstParameter::gameResolutionY/2){
					picY=CommonConstParameter::gameResolutionY/2;
				} else{
					picY=20;
				}
				m_stageInfoVec[lock->stageIndex].DrawInfo(picX,picY,m_stageNameFont,m_explainFont);
			}
		}
	}
}