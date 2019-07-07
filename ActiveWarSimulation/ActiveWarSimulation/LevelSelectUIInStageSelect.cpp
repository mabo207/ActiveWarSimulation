#include"LevelSelectUIInStageSelect.h"
#include"DxLib.h"
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"

namespace {
	const int levelBoxWidth=CommonConstParameter::gameResolutionX/4;
	const int levelBoxHeight=CommonConstParameter::gameResolutionY/4-80;
	int GetStageInfoTargetX(){
		return StageInfoInStageSelect::boxWidth/2+10;
	}
	int GetStageInfoTargetY(){
		return StageInfoInStageSelect::boxHeight/2+10;
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
	,m_levelButtonX(slideInOutLevelBoxTargetX,CommonConstParameter::gameResolutionX-infoDrawAreaWidth/2-levelBoxWidth/2,slideInOutFrame,Easing::TYPE_OUT,Easing::FUNCTION_LINER,1.0)
	,m_stageInfoCenterPos(-StageInfoInStageSelect::boxWidth,GetStageInfoTargetX(),GetStageInfoTargetY(),GetStageInfoTargetY(),slideInOutFrame,Easing::TYPE_OUT,Easing::FUNCTION_LINER,1.0)
	,m_levelButton{MouseButtonUI(m_levelButtonX.GetX(),90,levelBoxWidth,levelBoxHeight,-1)
		,MouseButtonUI(m_levelButtonX.GetX(),110+levelBoxHeight,levelBoxWidth,levelBoxHeight,-1)
		,MouseButtonUI(m_levelButtonX.GetX(),130+levelBoxHeight*2,levelBoxWidth,levelBoxHeight,-1)
		,MouseButtonUI(m_levelButtonX.GetX(),150+levelBoxHeight*3,levelBoxWidth,levelBoxHeight,-1)}
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
	m_stageInfo.DrawInfo(m_stageInfoCenterPos.GetX(),m_stageInfoCenterPos.GetY(),m_stageNameFont,m_explainFont);
	//右側にレベル選択を表示
	for(size_t i=0;i<StageLevel::levelCount;i++){
		int x,y;
		m_levelButton[i].GetButtonInfo(&x,&y,nullptr,nullptr);
		const StageLevel level=StageLevel::levelArray[i];
		//背景の描画
		m_levelButton[i].DrawButtonRect(GetColor(64,32,32),TRUE);
		//レベル名の描画
		DrawStringToHandle(x+5,y+5,level.GetString().c_str(),GetColor(255,255,255),m_explainFont);
		//ランキングデータ一覧
		const std::map<StageLevel,ScoreRankingData::LevelData>::const_iterator itLevel=m_stageInfo.m_rankingVec.levelMap.find(level);
		size_t counter=0;
		const size_t rankingSize=5;
		const int nameX=10,scoreX=350;
		int rankingY=40;
		if(itLevel!=m_stageInfo.m_rankingVec.levelMap.end()){
			//ランキングデータが存在する場合はデータを描画
			std::set<ScoreRankingData::PlayerData>::const_iterator itPlayer;
			for(const ScoreRankingData::PlayerData &data:itLevel->second.playerDataSet){
				//名前
				DrawStringToHandle(x+nameX,y+rankingY,data.name.c_str(),GetColor(255,255,255),m_explainFont);
				//点数
				DrawStringRightJustifiedToHandle(x+scoreX,y+rankingY,to_string_0d(data.score,7),GetColor(255,255,255),m_explainFont);
				//位置ずらし
				rankingY+=explainFontSize;
				//個数を増やして、rankingSize個以上の描画になったら描画を打ち切る
				counter++;
				if(counter>=rankingSize){
					break;
				}
			}
		}
		for(;counter<rankingSize;counter++){
			//足りない分は-----を描画
			//名前
			DrawStringToHandle(x+nameX,y+rankingY,"----------",GetColor(255,255,255),m_explainFont);
			//点数
			DrawStringRightJustifiedToHandle(x+scoreX,y+rankingY,"-------",GetColor(255,255,255),m_explainFont);
			//位置ずらし
			rankingY+=explainFontSize;
		}
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