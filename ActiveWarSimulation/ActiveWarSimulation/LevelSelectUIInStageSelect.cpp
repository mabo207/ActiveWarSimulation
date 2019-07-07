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
}

//-------------------LevelSelectUIInStageSelect----------------------
LevelSelectUIInStageSelect::LevelSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData
	,const StageInfoInStageSelect &stageInfo
	,int stageNameFont
	,int explainFont
)
	:BaseUIInStageSelect(controledData)
	,m_stageInfo(stageInfo)
	,m_levelButtonX(slideInOutLevelBoxTargetX,CommonConstParameter::gameResolutionX-infoDrawAreaWidth/2-StageInfoInStageSelect::levelBoxWidth/2,slideInOutFrame,Easing::TYPE_OUT,Easing::FUNCTION_LINER,1.0)
	,m_stageInfoCenterPos(-StageInfoInStageSelect::stageBoxWidth,GetStageInfoTargetX(),GetStageInfoTargetY(),GetStageInfoTargetY(),slideInOutFrame,Easing::TYPE_OUT,Easing::FUNCTION_LINER,1.0)
	,m_levelButton{MouseButtonUI(m_levelButtonX.GetX(),90,StageInfoInStageSelect::StageInfoInStageSelect::levelBoxWidth,StageInfoInStageSelect::levelBoxHeight,-1)
		,MouseButtonUI(m_levelButtonX.GetX(),110+StageInfoInStageSelect::levelBoxHeight,StageInfoInStageSelect::levelBoxWidth,StageInfoInStageSelect::levelBoxHeight,-1)
		,MouseButtonUI(m_levelButtonX.GetX(),130+StageInfoInStageSelect::levelBoxHeight*2,StageInfoInStageSelect::levelBoxWidth,StageInfoInStageSelect::levelBoxHeight,-1)
		,MouseButtonUI(m_levelButtonX.GetX(),150+StageInfoInStageSelect::levelBoxHeight*3,StageInfoInStageSelect::levelBoxWidth,StageInfoInStageSelect::levelBoxHeight,-1)}
	,m_stageNameFont(stageNameFont)
	,m_explainFont(explainFont)
{}

LevelSelectUIInStageSelect::~LevelSelectUIInStageSelect(){}

BaseUIInStageSelect::UpdateResult LevelSelectUIInStageSelect::Update(){
	//UIの位置更新
	m_stageInfoCenterPos.Update();
	if(!m_levelButtonX.GetEndFlag()){
		m_levelButtonX.Update();
		for(MouseButtonUI &ui:m_levelButton){
			int y;
			ui.GetButtonInfo(nullptr,&y,nullptr,nullptr);
			ui.WarpTo(m_levelButtonX.GetX(),y);
		}
	}
	//レベル選択更新処理
	bool levelUpdate=false;
	bool mouseInLevelBox=false;
	bool levelBoxTap=false;
	const Vector2D mousePos=GetMousePointVector2D();//現在のフレームのマウスの位置
	if(!m_controledData.expired()){
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const StageLevel beforeLevel=lock->selectLevel;
		if(keyboard_get(KEY_INPUT_UP)==1){
			lock->selectLevel=beforeLevel.Shift(-1);
		} else if(keyboard_get(KEY_INPUT_DOWN)==1){
			lock->selectLevel=beforeLevel.Shift(1);
		} else{
			//マウスによる更新処理
			for(size_t i=0;i<StageLevel::levelCount;i++){
				if(m_levelButton[i].JudgeIn()){
					if((mousePos-m_beforeFrameMousePos).sqSize()>=4.0f){
						//一定距離以上マウスを動かさないと更新されない
						lock->selectLevel=StageLevel::levelArray[i];
					}
					mouseInLevelBox=(lock->selectLevel==StageLevel::levelArray[i]);//マウスが指しているレベルと選択しているレベルが一致しているか
					break;
				}
			}
		}
		levelUpdate=!(lock->selectLevel==beforeLevel);//StageLevelには!=が存在しない
	}
	if(levelUpdate){
		//レベル更新が行われた場合は効果音を鳴らす
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);
	} else{
		//その他のUI処理(レベル更新が行われた時はこれらの処理はしないようにする)
		if(keyboard_get(KEY_INPUT_Z)==1
			|| (mouseInLevelBox && mouse_get(MOUSE_INPUT_LEFT)==1))
		{
			//次の選択へ
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
			return UpdateResult::e_gotoBattle;
		} else if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1){
			//戻る
			PlaySoundMem(GeneralPurposeResource::cancelSound,DX_PLAYTYPE_BACK,TRUE);
			return UpdateResult::e_gotoStageSelect;
		}
	}
	//m_beforeFrameMousePosの更新
	m_beforeFrameMousePos=mousePos;

	return UpdateResult::e_notTransition;
}

void LevelSelectUIInStageSelect::Draw()const{
	const int explainFontSize=GetFontSizeToHandle(m_explainFont);
	//背景を少し暗くする
	{
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(mode,pal);
	}
	//左側にステージを表示
	m_stageInfo.DrawStageInfo(m_stageInfoCenterPos.GetX(),m_stageInfoCenterPos.GetY(),m_stageNameFont,m_explainFont);
	//右側にレベル選択を表示
	for(size_t i=0;i<StageLevel::levelCount;i++){
		int x,y;
		m_levelButton[i].GetButtonInfo(&x,&y,nullptr,nullptr);
		const StageLevel level=StageLevel::levelArray[i];
		m_stageInfo.DrawLevelInfo(level,x,y,m_explainFont,m_explainFont);

	}
	//選択しているレベルを強調
	if(!m_controledData.expired()){
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t index=lock->selectLevel.GetIndex();
		if(index<StageLevel::levelCount){
			m_levelButton[index].DrawButtonRect(GetColor(196,255,64),FALSE,5);
		}
	}
}