#include"GeneralPurposeResourceManager.h"
#include"DxLib.h"
#include"FilePath.h"

//---------------------GeneralPurposeResourceManager---------------------
int GeneralPurposeResourceManager::cancelSound=-1;
int GeneralPurposeResourceManager::decideSound=-1;
int GeneralPurposeResourceManager::selectSound=-1;

void GeneralPurposeResourceManager::LoadResource(){
	//画像

	//フォント

	//音
	decideSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/decide.ogg").c_str());
	cancelSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/cancel.ogg").c_str());
	selectSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/select.ogg").c_str());
}

void GeneralPurposeResourceManager::ReleaseResource(){
	//画像

	//フォント

	//音
	DeleteSoundMem(decideSound);
	DeleteSoundMem(cancelSound);
	DeleteSoundMem(selectSound);
}


