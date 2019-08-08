#define NOMINMAX	//std::max()�̏Փ˂̉��
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
	const int slideInOutFrame=15;//���ڂ��E���ɓ����Ă���܂ł̃A�j���[�V�����̎���
	const int selectAnimationFrame=30;//�X�e�[�W��ύX�������̃A�j���[�V�����̎���
	//���������ɂ���Ă͂��܂���������Ȃ��\���̂���p�����[�^��S�Ċ֐�������
	//1�̃X�e�[�W���`��ŗp����y�����̕�
	int GetInfoDY(){
		return StageInfoInStageSelect::stageBoxHeight+offsetY;
	}
	//�E���̈ړ��ڕW�ʒu
	int GetTargetX(){
		return CommonConstParameter::gameResolutionX-BaseUIInStageSelect::infoDrawAreaWidth/2;
	}
	int GetTargetY(){
		return CommonConstParameter::gameResolutionY/2;
	}
	//���ڂ̃X���C�h�C���J�n�ʒu�E�X���C�h�A�E�g�I���ʒu
	int GetSlideInOutX(){
		return CommonConstParameter::gameResolutionX+BaseUIInStageSelect::infoDrawAreaWidth;
	}
}

//----------------------StageSelectUIInStageSelect-----------------------
StageSelectUIInStageSelect::StageSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData
	,const MouseButtonUI &backButton
	,const std::vector<StageInfoInStageSelect> &stageInfoVec
	,int stageNameFont
	,int explainFont
	,size_t clearStageNum
)
	:BaseUIInStageSelect(controledData,backButton)
	,m_beforeFrameMousePos(GetMousePointVector2D())
	,m_stageInfoVec(stageInfoVec)
	,m_stageNameFont(stageNameFont)
	,m_explainFont(explainFont)
	,m_upButton(CommonConstParameter::gameResolutionX-infoDrawAreaWidth,0,infoDrawAreaWidth,buttonHeight,LoadGraphEX(FilePath::graphicDir+"countUp.png"))
	,m_downButton(CommonConstParameter::gameResolutionX-infoDrawAreaWidth,CommonConstParameter::gameResolutionY-buttonHeight,infoDrawAreaWidth,buttonHeight,LoadGraphEX(FilePath::graphicDir+"countDown.png"))
	,m_selectStageButton(GetTargetX()-StageInfoInStageSelect::stageBoxWidth/2,GetTargetY()-StageInfoInStageSelect::stageBoxHeight/2,StageInfoInStageSelect::stageBoxWidth,StageInfoInStageSelect::stageBoxHeight,-1)
	,m_selectStagePos(GetSlideInOutX(),GetTargetX(),GetTargetY(),GetTargetY(),slideInOutFrame,Easing::TYPE_OUT,Easing::FUNCTION_LINER,9.0)//�ŏ��͉E��������Ă���悤�ȉ��o
	,m_afterDicide(false)
	,m_stageNum(std::min(clearStageNum+1,stageInfoVec.size()))
{}

StageSelectUIInStageSelect::~StageSelectUIInStageSelect(){}

BaseUIInStageSelect::UpdateResult StageSelectUIInStageSelect::Update(){
	bool indexUpdate=false;
	bool mouseInStage=false;
	const Vector2D mousePos=GetMousePointVector2D();//���̃t���[���̃}�E�X�̈ʒu
	const bool mouseMoveFlag=((mousePos-m_beforeFrameMousePos).sqSize()>=4.0f);//���̃t���[���Ń}�E�X�̈ړ����s������
	if(!m_controledData.expired()){
		//�`��ʒu�X�V
		m_selectStagePos.Update();
		if(m_selectStagePos.GetEndFlag()){
			if(!m_afterDicide){
				//�`��ʒu�̈ړ����I�������Am_beforeSelectStageIndex�����̃X�e�[�W�ɍ��킹�Ă���
				m_beforeSelectStageIndex=m_controledData.lock()->stageIndex;
			} else{
				//���ڂ̃X���C�h�A�E�g���I������ꍇ�A���x���Z���N�g�ɐi�܂���
				return UpdateResult::e_gotoLevelSelect;
			}
		}
		if(!m_afterDicide){
			//�I���X�e�[�W�̍X�V
			const std::shared_ptr<ControledData> lock=m_controledData.lock();
			const size_t beforeIndex=lock->stageIndex;//���ʉ��Đ��̉۔���ɗp����
			//�X�V����
			if(m_stageNum>0){
				//�\���L�[�ł̐؂�ւ�
				if(keyboard_get(KEY_INPUT_UP)==1 || m_upButton.JudgePressMoment()){
					lock->stageIndex=(lock->stageIndex+m_stageNum-1)%m_stageNum;
				} else if(keyboard_get(KEY_INPUT_DOWN)==1 || m_downButton.JudgePressMoment()){
					lock->stageIndex=(lock->stageIndex+1)%m_stageNum;
				} else{
					//�}�E�X�ł̐؂�ւ�
					const float circleSize=30.0f;//�����蔻��̉~�̑傫��
					for(size_t i=0;i<m_stageNum;i++){
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
			if(indexUpdate){
				//�ύX������Ό��ʉ��Đ�
				PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);
				//m_beforeSelectStageIndex�̍X�V
				m_beforeSelectStageIndex=beforeIndex;
				//m_selectStagePos�̍Đݒ�
				const int gapIndex=lock->stageIndex-beforeIndex;
				const int gapDY=gapIndex*GetInfoDY();
				const int selectStageNowY=m_selectStagePos.GetY()+gapDY;
				m_selectStagePos=PositionControl(m_selectStagePos.GetX(),GetTargetX(),selectStageNowY,GetTargetY(),selectAnimationFrame,Easing::TYPE_OUT,Easing::FUNCTION_EXPO,9.0);
			}
		}
	}
	//���̑��̓��͏���
	if(!indexUpdate && !m_afterDicide){
		//�ȉ��̑J�ڂ͑I���X�e�[�W�̕ύX���s���Ă��Ȃ������X�e�[�W����O�݂̂ŉ\
		//�X�e�[�W�̌���
		if(keyboard_get(KEY_INPUT_Z)==1
			|| (mouse_get(MOUSE_INPUT_LEFT)==1 && mouseInStage)
			|| m_selectStageButton.JudgePressMoment())
		{
			//���艹���o��
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
			//���ڂ��X���C�h�A�E�g������
			m_afterDicide=true;
			m_selectStagePos=PositionControl(m_selectStagePos.GetX(),GetSlideInOutX(),m_selectStagePos.GetY(),m_selectStagePos.GetendY(),slideInOutFrame,Easing::TYPE_OUT,Easing::FUNCTION_LINER,9.0);
		}
		//�߂�
		if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1 || m_backButton.JudgePressMoment()){
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
		const size_t infoVecSize=m_stageNum;//m_stageNum��m_stageInfoVec.size()�ȉ��ł��邱�Ƃ͕ۏ؂���Ă���
		const size_t selectIndex=lock->stageIndex;
		if(selectIndex<infoVecSize){
			//�ꏊ�̋���
			const Vector2D pos=m_stageInfoVec[selectIndex].m_pos;
			DrawCircleAA(pos.x,pos.y,30,10,GetColor(255,255,255),TRUE);
			//����`�悷��X�e�[�W�̌���(�I���X�e�[�W�̑O��e2���炢�͕`�悷��)
			size_t startIndex,endIndex;
			if(m_beforeSelectStageIndex<selectIndex){
				startIndex=m_beforeSelectStageIndex;
				endIndex=selectIndex;
			} else{
				startIndex=selectIndex;
				endIndex=m_beforeSelectStageIndex;
			}
			if(startIndex<displayOneSideInfoCount){
				startIndex=0;
			} else{
				startIndex-=displayOneSideInfoCount;
			}
			if(endIndex+displayOneSideInfoCount>=infoVecSize){
				endIndex=infoVecSize-1;
			} else{
				endIndex+=displayOneSideInfoCount;
			}
			//�X�e�[�W���̕`��
			for(size_t i=startIndex;i<=endIndex;i++){
				const int gap=i-selectIndex;
				const int centerX=m_selectStagePos.GetX();
				const int centerY=m_selectStagePos.GetY()+gap*GetInfoDY();
				if(i==selectIndex){
					//�I�����Ă���X�e�[�W�Ȃ�A���ʂɕ`��
					m_stageInfoVec[i].DrawStageInfo(centerX,centerY,m_stageNameFont,m_explainFont);
				} else{
					//�I�����Ă��Ȃ��X�e�[�W�Ȃ�A�ڗ����Ȃ��悤�ɕ`��
					int mode,pal;
					GetDrawBlendMode(&mode,&pal);
					SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
					m_stageInfoVec[i].DrawStageInfo(centerX,centerY,m_stageNameFont,m_explainFont);
					SetDrawBlendMode(mode,pal);
				}
			}
		}
	}
	//�{�^���̕`��
	m_upButton.DrawButton();
	m_downButton.DrawButton();
}