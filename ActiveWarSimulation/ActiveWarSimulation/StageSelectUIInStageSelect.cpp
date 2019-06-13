#include"DxLib.h"
#include"StageSelectUIInStageSelect.h"
#include"input.h"
#include"GeneralPurposeResourceManager.h"

StageSelectUIInStageSelect::StageSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData,const size_t stageNum)
	:BaseUIInStageSelect(controledData)
	,m_stageNum(stageNum)
{}

StageSelectUIInStageSelect::~StageSelectUIInStageSelect(){}

BaseUIInStageSelect::UpdateResult StageSelectUIInStageSelect::Update(){
	//選択ステージの更新
	bool indexUpdate=false;
	if(!m_controledData.expired()){
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t beforeIndex=lock->stageIndex;//効果音再生の可否判定に用いる
		if(m_stageNum>0){
			if(keyboard_get(KEY_INPUT_LEFT)==1){
				lock->stageIndex=(lock->stageIndex+m_stageNum-1)%m_stageNum;
			} else if(keyboard_get(KEY_INPUT_RIGHT)==1){
				lock->stageIndex=(lock->stageIndex+1)%m_stageNum;
			}
		}
		indexUpdate=(lock->stageIndex!=beforeIndex);//indexが更新されているならtrueに
	}
	if(indexUpdate){
		//変更があれば効果音再生
		PlaySoundMem(GeneralPurposeResourceManager::selectSound,DX_PLAYTYPE_BACK,TRUE);
	} else{
		//以下の遷移は選択ステージの変更が行われていない時のみできる
		//ステージの決定
		if(keyboard_get(KEY_INPUT_Z)==1){
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

}

std::shared_ptr<BaseUIInStageSelect> StageSelectUIInStageSelect::GetNextUI(const std::weak_ptr<ControledData> &controledData)const{
	return std::shared_ptr<BaseUIInStageSelect>();
}