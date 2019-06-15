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
	//選択ステージの更新
	bool indexUpdate=false;
	bool mouseInStage=false;//マウスがステージを示す円の中に入っているかどうか
	if(!m_controledData.expired()){
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t beforeIndex=lock->stageIndex;//効果音再生の可否判定に用いる
		if(m_stageNum>0){
			//十字キーでの切り替え
			if(keyboard_get(KEY_INPUT_LEFT)==1){
				lock->stageIndex=(lock->stageIndex+m_stageNum-1)%m_stageNum;
			} else if(keyboard_get(KEY_INPUT_RIGHT)==1){
				lock->stageIndex=(lock->stageIndex+1)%m_stageNum;
			} else{
				//マウスでの切り替え
				const Vector2D mousePos=GetMousePointVector2D();
				const float circleSize=30.0f;//当たり判定の円の大きさ
				for(size_t i=0;i<m_stageNum;i++){
					if((m_stageInfoVec[i].m_pos-mousePos).sqSize()<=circleSize*circleSize){
						lock->stageIndex=i;
						mouseInStage=true;
						break;
					}
				}
			}
		}
		indexUpdate=(lock->stageIndex!=beforeIndex);//indexが更新されているならtrueに
	}
	//選択以外の入力処理
	if(indexUpdate){
		//変更があれば効果音再生
		PlaySoundMem(GeneralPurposeResourceManager::selectSound,DX_PLAYTYPE_BACK,TRUE);
	} else{
		//以下の遷移は選択ステージの変更が行われていない時のみできる
		//ステージの決定
		if(keyboard_get(KEY_INPUT_Z)==1
			|| (mouse_get(MOUSE_INPUT_LEFT)==1 && mouseInStage))
		{
			//決定音を出す
			PlaySoundMem(GeneralPurposeResourceManager::decideSound,DX_PLAYTYPE_BACK,TRUE);
			//遷移
			return UpdateResult::e_gotoBattle;
		}
		//戻る
		if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1){
			//back音を出す
			PlaySoundMem(GeneralPurposeResourceManager::cancelSound,DX_PLAYTYPE_BACK,TRUE);
			//遷移
			return UpdateResult::e_gotoTitle;
		}
	}

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
			//ステージ絵の描画
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
			//ステージ情報の描画
			
		}
	}
}

std::shared_ptr<BaseUIInStageSelect> StageSelectUIInStageSelect::GetNextUI(const std::weak_ptr<ControledData> &controledData)const{
	return std::shared_ptr<BaseUIInStageSelect>();
}