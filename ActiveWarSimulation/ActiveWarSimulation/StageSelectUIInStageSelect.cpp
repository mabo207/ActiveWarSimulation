#include"DxLib.h"
#include"StageSelectUIInStageSelect.h"
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"
#include"GraphicControl.h"
#include"FilePath.h"

namespace {
	const size_t displayOneSideInfoCount=2;//片方に余分にいくつ表示するか
	const int offsetY=20;//ステージ情報描画UI間の隙間
	const int buttonHeight=80;//ボタンの高さ(非同期読み込みを想定すると、画像から高さを求めるのが困難)
	//初期化順によってはうまく処理されない可能性のあるパラメータを全て関数化する
	//1つのステージ情報描画で用いるy方向の幅
	int GetInfoDY(){
		return StageInfoInStageSelect::boxHeight+offsetY;
	}
	//右側の移動目標位置
	int GetTargetX(){
		return CommonConstParameter::gameResolutionX-BaseUIInStageSelect::infoDrawAreaWidth/2;
	}
	int GetTargetY(){
		return CommonConstParameter::gameResolutionY/2;
	}
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
	,m_selectStageButton(GetTargetX()-StageInfoInStageSelect::boxWidth/2,GetTargetY()-StageInfoInStageSelect::boxHeight/2,StageInfoInStageSelect::boxWidth,StageInfoInStageSelect::boxHeight,-1)
	,m_selectStagePos(GetTargetX(),GetTargetY(),30,Easing::TYPE_OUT,Easing::FUNCTION_EXPO,9.0)
{}

StageSelectUIInStageSelect::~StageSelectUIInStageSelect(){}

BaseUIInStageSelect::UpdateResult StageSelectUIInStageSelect::Update(){
	bool indexUpdate=false;
	bool mouseInStage=false;
	const Vector2D mousePos=GetMousePointVector2D();//今のフレームのマウスの位置
	const bool mouseMoveFlag=((mousePos-m_beforeFrameMousePos).sqSize()>=4.0f);//このフレームでマウスの移動を行ったか
	if(!m_controledData.expired()){
		//描画位置更新
		m_selectStagePos.Update();
		if(m_selectStagePos.GetEndFlag()){
			//描画位置の移動が終わったら、m_beforeSelectStageIndexを今のステージに合わせておく
			m_beforeSelectStageIndex=m_controledData.lock()->stageIndex;
		}
		//選択ステージの更新
		const size_t stageNum=m_stageInfoVec.size();
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t beforeIndex=lock->stageIndex;//効果音再生の可否判定に用いる
		//更新処理
		if(stageNum>0){
			//十字キーでの切り替え
			if(keyboard_get(KEY_INPUT_UP)==1 || m_upButton.JudgePressMoment()){
				lock->stageIndex=(lock->stageIndex+stageNum-1)%stageNum;
			} else if(keyboard_get(KEY_INPUT_DOWN)==1 || m_downButton.JudgePressMoment()){
				lock->stageIndex=(lock->stageIndex+1)%stageNum;
			} else{
				//マウスでの切り替え
				const float circleSize=30.0f;//当たり判定の円の大きさ
				for(size_t i=0;i<stageNum;i++){
					if((m_stageInfoVec[i].m_pos-mousePos).sqSize()<=circleSize*circleSize){
						if(mouseMoveFlag){
							//マウスを一定距離以上動かした場合のみ更新
							lock->stageIndex=i;
						}
						mouseInStage=(lock->stageIndex==i);//マウスが指しているステージと現在選択しているステージが一致しているか
						break;
					}
				}
			}
		}
		//更新結果について記録
		indexUpdate=(lock->stageIndex!=beforeIndex);//indexが更新されているならtrueに
		if(indexUpdate){
			//変更があれば効果音再生
			PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);
			//m_beforeSelectStageIndexの更新
			m_beforeSelectStageIndex=beforeIndex;
			//m_selectStagePosの再設定
			const int gapIndex=lock->stageIndex-beforeIndex;
			const int gapDY=gapIndex*GetInfoDY();
			const int selectStageNowY=m_selectStagePos.GetY()+gapDY;
			m_selectStagePos=PositionControl(m_selectStagePos.GetX(),GetTargetX(),selectStageNowY,GetTargetY(),m_selectStagePos.GetMaxFrame(),m_selectStagePos.GetType(),m_selectStagePos.GetFunction(),m_selectStagePos.GetDegree());
		}
	}
	//その他の入力処理
	if(!indexUpdate){
		//以下の遷移は選択ステージの変更が行われていない時のみできる
		//ステージの決定
		if(keyboard_get(KEY_INPUT_Z)==1
			|| (mouse_get(MOUSE_INPUT_LEFT)==1 && mouseInStage)
			|| m_selectStageButton.JudgePressMoment())
		{
			//決定音を出す
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
			//遷移
			return UpdateResult::e_gotoLevelSelect;
		}
		//戻る
		if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1){
			//back音を出す
			PlaySoundMem(GeneralPurposeResource::cancelSound,DX_PLAYTYPE_BACK,TRUE);
			//遷移
			return UpdateResult::e_gotoTitle;
		}
	}
	//m_beforeFrameMousePosの更新
	m_beforeFrameMousePos=mousePos;

	return UpdateResult::e_notTransition;
}

void StageSelectUIInStageSelect::Draw()const{
	if(!m_controledData.expired()){
		//選択しているステージの表示と場所の強調
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t infoVecSize=m_stageInfoVec.size();
		const size_t selectIndex=lock->stageIndex;
		if(selectIndex<infoVecSize){
			//場所の協調
			const Vector2D pos=m_stageInfoVec[selectIndex].m_pos;
			DrawCircleAA(pos.x,pos.y,30,10,GetColor(255,255,255),TRUE);
			//情報を描画するステージの決定(選択ステージの前後各2つくらいは描画する)
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
			//ステージ情報の描画
			for(size_t i=startIndex;i<=endIndex;i++){
				const int gap=i-selectIndex;
				const int centerX=m_selectStagePos.GetX();
				const int centerY=m_selectStagePos.GetY()+gap*GetInfoDY();
				if(i==selectIndex){
					//選択しているステージなら、普通に描画
					m_stageInfoVec[i].DrawInfo(centerX,centerY,m_stageNameFont,m_explainFont);
				} else{
					//選択していないステージなら、目立たないように描画
					int mode,pal;
					GetDrawBlendMode(&mode,&pal);
					SetDrawBlendMode(DX_BLENDMODE_ALPHA,64);
					m_stageInfoVec[i].DrawInfo(centerX,centerY,m_stageNameFont,m_explainFont);
					SetDrawBlendMode(mode,pal);
				}
			}
		}
	}
	//ボタンの描画
	m_upButton.DrawButton();
	m_downButton.DrawButton();
}