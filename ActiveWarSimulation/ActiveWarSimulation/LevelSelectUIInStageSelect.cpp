#include"LevelSelectUIInStageSelect.h"
#include"DxLib.h"
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"

namespace {
	int GetStageInfoTargetX(){
		return StageInfoInStageSelect::stageBoxWidth/2+10;
	}
	int GetStageInfoTargetY(){
		return StageInfoInStageSelect::stageBoxHeight/2+10;
	}
	const int slideInOutLevelBoxTargetX=CommonConstParameter::gameResolutionX*6/5;
	const int slideInOutFrame=15;
	const int offsetY=20;
}

//-------------------LevelSelectUIInStageSelect----------------------
LevelSelectUIInStageSelect::LevelSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData
	,const MouseButtonUI &backButton
	,const StageInfoInStageSelect &stageInfo
	,int stageNameFont
	,int explainFont
)
	:BaseUIInStageSelect(controledData,backButton)
	,m_stageInfo(stageInfo)
	,m_levelButtonX(slideInOutLevelBoxTargetX,CommonConstParameter::gameResolutionX-infoDrawAreaWidth/2-StageInfoInStageSelect::levelBoxWidth/2,slideInOutFrame,Easing::TYPE_OUT,Easing::FUNCTION_LINER,1.0)
	,m_stageInfoCenterPos(-StageInfoInStageSelect::stageBoxWidth,GetStageInfoTargetX(),GetStageInfoTargetY(),GetStageInfoTargetY(),slideInOutFrame,Easing::TYPE_OUT,Easing::FUNCTION_LINER,1.0)
	,m_levelButton{MouseButtonUI(m_levelButtonX.GetX(),offsetY,StageInfoInStageSelect::StageInfoInStageSelect::levelBoxWidth,StageInfoInStageSelect::levelBoxHeight,-1)
		,MouseButtonUI(m_levelButtonX.GetX(),offsetY+(StageInfoInStageSelect::levelBoxHeight+offsetY),StageInfoInStageSelect::levelBoxWidth,StageInfoInStageSelect::levelBoxHeight,-1)
		,MouseButtonUI(m_levelButtonX.GetX(),offsetY+(StageInfoInStageSelect::levelBoxHeight+offsetY)*2,StageInfoInStageSelect::levelBoxWidth,StageInfoInStageSelect::levelBoxHeight,-1)
		,MouseButtonUI(m_levelButtonX.GetX(),offsetY+(StageInfoInStageSelect::levelBoxHeight+offsetY)*3,StageInfoInStageSelect::levelBoxWidth,StageInfoInStageSelect::levelBoxHeight,-1)}
	,m_afterTransitionAction(false)
	,m_transitionResult(UpdateResult::e_gotoStageSelect)//�e�L�g�[�ɏ�����
	,m_stageNameFont(stageNameFont)
	,m_explainFont(explainFont)
{}

LevelSelectUIInStageSelect::~LevelSelectUIInStageSelect(){}

BaseUIInStageSelect::UpdateResult LevelSelectUIInStageSelect::Update(){
	//UI�̈ʒu�X�V
	m_stageInfoCenterPos.Update();
	if(!m_levelButtonX.GetEndFlag()){
		m_levelButtonX.Update();
		for(MouseButtonUI &ui:m_levelButton){
			int y;
			ui.GetButtonInfo(nullptr,&y,nullptr,nullptr);
			ui.WarpTo(m_levelButtonX.GetX(),y);
		}
	} else if(m_afterTransitionAction){
		//���ڌ����Ɉړ����I�������J��
		return m_transitionResult;
	}
	//���x���I���X�V����(m_afterTransitionAction���͍X�V����Ȃ�)
	if(!m_afterTransitionAction){
		bool levelUpdate=false;
		bool mouseInLevelBox=false;
		bool levelBoxTap=false;
		const Vector2D mousePos=GetMousePointVector2D();//���݂̃t���[���̃}�E�X�̈ʒu
		if(!m_controledData.expired()){
			const std::shared_ptr<ControledData> lock=m_controledData.lock();
			const StageLevel beforeLevel=lock->selectLevel;
			if(keyboard_get(KEY_INPUT_UP)==1){
				lock->selectLevel=beforeLevel.Shift(-1);
			} else if(keyboard_get(KEY_INPUT_DOWN)==1){
				lock->selectLevel=beforeLevel.Shift(1);
			} else{
				//�}�E�X�ɂ��X�V����
				for(size_t i=0;i<StageLevel::levelCount;i++){
					if(m_levelButton[i].JudgeIn()){
						if((mousePos-m_beforeFrameMousePos).sqSize()>=4.0f){
							//��苗���ȏ�}�E�X�𓮂����Ȃ��ƍX�V����Ȃ�
							lock->selectLevel=StageLevel::levelArray[i];
						}
						mouseInLevelBox=(lock->selectLevel==StageLevel::levelArray[i]);//�}�E�X���w���Ă��郌�x���ƑI�����Ă��郌�x������v���Ă��邩
						break;
					}
				}
			}
			levelUpdate=!(lock->selectLevel==beforeLevel);//StageLevel�ɂ�!=�����݂��Ȃ�
		}
		if(levelUpdate){
			//���x���X�V���s��ꂽ�ꍇ�͌��ʉ���炷
			PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);
		} else{
			//���̑���UI����(���x���X�V���s��ꂽ���͂����̏����͂��Ȃ��悤�ɂ���)
			if(keyboard_get(KEY_INPUT_Z)==1
				|| (mouseInLevelBox && mouse_get(MOUSE_INPUT_LEFT)==1))
			{
				//���ʉ���炷
				PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
				//�����̏�Ԃɂ���(�o�g���֐i��)
				BecomeAfterTransitionAction(UpdateResult::e_gotoBattle);
			} else if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1 || m_backButton.JudgePressMoment()){
				//���ʉ���炷
				PlaySoundMem(GeneralPurposeResource::cancelSound,DX_PLAYTYPE_BACK,TRUE);
				//�����̏�Ԃɂ���(�X�e�[�W�Z���N�g�ɖ߂�)
				BecomeAfterTransitionAction(UpdateResult::e_gotoStageSelect);
			}
		}
		//m_beforeFrameMousePos�̍X�V
		m_beforeFrameMousePos=mousePos;
	}

	return UpdateResult::e_notTransition;
}

void LevelSelectUIInStageSelect::Draw()const{
	const int explainFontSize=GetFontSizeToHandle(m_explainFont);
	//�w�i�������Â�����
	{
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(mode,pal);
	}
	//�����ɃX�e�[�W��\��
	m_stageInfo.DrawStageInfo(m_stageInfoCenterPos.GetX(),m_stageInfoCenterPos.GetY(),m_stageNameFont,m_explainFont);
	//�E���Ƀ��x���I����\��
	for(size_t i=0;i<StageLevel::levelCount;i++){
		int x,y;
		m_levelButton[i].GetButtonInfo(&x,&y,nullptr,nullptr);
		const StageLevel level=StageLevel::levelArray[i];
		m_stageInfo.DrawLevelInfo(level,x,y,m_stageNameFont,m_explainFont);

	}
	//�I�����Ă��郌�x��������
	if(!m_controledData.expired()){
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t index=lock->selectLevel.GetIndex();
		if(index<StageLevel::levelCount){
			m_levelButton[index].DrawButtonRect(GetColor(196,255,64),FALSE,5);
		}
	}
}

void LevelSelectUIInStageSelect::BecomeAfterTransitionAction(UpdateResult result){
	m_afterTransitionAction=true;
	m_transitionResult=result;
	//m_levelButtonX��m_stageInfoCenterPos���r���ňʒu�̕ύX�͍s���Ȃ����߁A�����ʒu�ɖ߂�悤�ɂ���΂悢
	m_levelButtonX.SetTarget(m_levelButtonX.GetstartX(),true);
	m_stageInfoCenterPos.SetTarget(m_stageInfoCenterPos.GetstartX(),m_stageInfoCenterPos.GetstartY(),true);
}