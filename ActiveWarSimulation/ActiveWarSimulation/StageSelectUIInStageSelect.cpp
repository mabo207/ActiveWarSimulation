#include"DxLib.h"
#include"StageSelectUIInStageSelect.h"
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"
#include"GraphicControl.h"
#include"FilePath.h"

namespace {
	const size_t displayOneSideInfoCount=2;//�Е��ɗ]���ɂ����\�����邩
	const int offsetY=20;//�X�e�[�W���`��UI�Ԃ̌���
	const int buttonHeight=80;//�{�^���̍���(�񓯊��ǂݍ��݂�z�肷��ƁA�摜���獂�������߂�̂�����)
}

//----------------------StageSelectUIInStageSelect-----------------------
StageSelectUIInStageSelect::StageSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData
	,const std::vector<StageInfoInStageSelect> &stageInfoVec
	,int stageNameFont
	,int explainFont
)
	:BaseUIInStageSelect(controledData)
	,m_beforeFrameMousePos(GetMousePointVector2D())
	,m_stageInfoVec(stageInfoVec)
	,m_stageNameFont(stageNameFont)
	,m_explainFont(explainFont)
	,m_upButton(CommonConstParameter::gameResolutionX-infoDrawAreaWidth,0,infoDrawAreaWidth,buttonHeight,LoadGraphEX((FilePath::graphicDir+"countUp.png").c_str()))
	,m_downButton(CommonConstParameter::gameResolutionX-infoDrawAreaWidth,CommonConstParameter::gameResolutionY-buttonHeight,infoDrawAreaWidth,buttonHeight,LoadGraphEX((FilePath::graphicDir+"countDown.png").c_str()))
	,m_selectStageButton(CommonConstParameter::gameResolutionX-infoDrawAreaWidth/2-StageInfoInStageSelect::boxWidth/2,CommonConstParameter::gameResolutionY/2-StageInfoInStageSelect::boxHeight/2,StageInfoInStageSelect::boxWidth,StageInfoInStageSelect::boxHeight,-1)
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
			if(keyboard_get(KEY_INPUT_UP)==1 || m_upButton.JudgePressMoment()){
				lock->stageIndex=(lock->stageIndex+stageNum-1)%stageNum;
			} else if(keyboard_get(KEY_INPUT_DOWN)==1 || m_downButton.JudgePressMoment()){
				lock->stageIndex=(lock->stageIndex+1)%stageNum;
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
			|| (mouse_get(MOUSE_INPUT_LEFT)==1 && mouseInStage)
			|| m_selectStageButton.JudgePressMoment())
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
			const size_t selectIndex=lock->stageIndex;
			//�ꏊ�̋���
			const Vector2D pos=m_stageInfoVec[selectIndex].m_pos;
			DrawCircleAA(pos.x,pos.y,30,10,GetColor(255,255,255),TRUE);
			//�X�e�[�W���̕`��(�I���X�e�[�W�̑O��e2���炢�͕`�悷��)
			const size_t startIndex=(selectIndex<displayOneSideInfoCount)?0:selectIndex-displayOneSideInfoCount;
			const size_t endIndex=(selectIndex+displayOneSideInfoCount<m_stageInfoVec.size())?selectIndex+displayOneSideInfoCount:m_stageInfoVec.size()-1;
			for(size_t i=startIndex;i<=endIndex;i++){
				const int gap=i-selectIndex;
				const int centerX=CommonConstParameter::gameResolutionX-infoDrawAreaWidth/2;
				const int centerY=CommonConstParameter::gameResolutionY/2+gap*(StageInfoInStageSelect::boxHeight+offsetY);
				if(i==selectIndex){
					//�I�����Ă���X�e�[�W�Ȃ�A���ʂɕ`��
					m_stageInfoVec[i].DrawInfo(centerX,centerY,m_stageNameFont,m_explainFont);
				} else{
					//�I�����Ă��Ȃ��X�e�[�W�Ȃ�A�ڗ����Ȃ��悤�ɕ`��
					int mode,pal;
					GetDrawBlendMode(&mode,&pal);
					SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
					m_stageInfoVec[i].DrawInfo(centerX,centerY,m_stageNameFont,m_explainFont);
					SetDrawBlendMode(mode,pal);
				}
			}
		}
	}
	//�{�^���̕`��
	m_upButton.DrawButton();
	m_downButton.DrawButton();
}