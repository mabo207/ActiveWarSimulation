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
	//選択ステージの更新
	bool indexUpdate=false;
	bool mouseInStage=false;
	const Vector2D mousePos=GetMousePointVector2D();//今のフレームのマウスの位置
	const bool mouseMoveFlag=((mousePos-m_beforeFrameMousePos).sqSize()>=4.0f);//このフレームでマウスの移動を行ったか
	if(!m_controledData.expired()){
		const size_t stageNum=m_stageInfoVec.size();
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t beforeIndex=lock->stageIndex;//効果音再生の可否判定に用いる
		//更新処理
		if(stageNum>0){
			//十字キーでの切り替え
			if(keyboard_get(KEY_INPUT_LEFT)==1){
				lock->stageIndex=(lock->stageIndex+stageNum-1)%stageNum;
				//m_drawStageInfoの更新
				m_drawStageInfo=true;
			} else if(keyboard_get(KEY_INPUT_RIGHT)==1){
				lock->stageIndex=(lock->stageIndex+1)%stageNum;
				//m_drawStageInfoの更新
				m_drawStageInfo=true;
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
				//m_drawStageInfoの更新
				if(mouseInStage){
					//マウスがステージの中にあればステージ選択していることにする
					m_drawStageInfo=true;
				} else if(mouseMoveFlag){
					//マウスを動かしたのにマウスがステージ選択をしていない場合はステージが何も選ばれていない状態にする
					m_drawStageInfo=false;
				}
			}
		}
		//更新結果について記録
		indexUpdate=(lock->stageIndex!=beforeIndex);//indexが更新されているならtrueに
	}
	//選択以外の入力処理
	if(indexUpdate){
		//変更があれば効果音再生
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);
	} else{
		//以下の遷移は選択ステージの変更が行われていない時のみできる
		//ステージの決定
		if(keyboard_get(KEY_INPUT_Z)==1
			|| (mouse_get(MOUSE_INPUT_LEFT)==1 && mouseInStage))
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
		if(lock->stageIndex<m_stageInfoVec.size()){
			//場所の協調
			const Vector2D pos=m_stageInfoVec[lock->stageIndex].m_pos;
			DrawCircleAA(pos.x,pos.y,30,10,GetColor(255,255,255),TRUE);
			//ステージ情報の描画
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