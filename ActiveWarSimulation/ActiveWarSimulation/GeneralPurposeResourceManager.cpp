#include"GeneralPurposeResourceManager.h"
#include"DxLib.h"

//---------------------GeneralPurposeResourceManager---------------------
int GeneralPurposeResourceManager::cancelSound=-1;
int GeneralPurposeResourceManager::decideSound=-1;
int GeneralPurposeResourceManager::selectSound=-1;

void GeneralPurposeResourceManager::LoadResource(){
	//�摜

	//�t�H���g

	//��
	decideSound=LoadSoundMem("sound/effect/nonfree/decide.ogg");
	cancelSound=LoadSoundMem("sound/effect/nonfree/cancel.ogg");
	selectSound=LoadSoundMem("sound/effect/nonfree/select.ogg");
}

void GeneralPurposeResourceManager::ReleaseResource(){
	//�摜

	//�t�H���g

	//��
	DeleteSoundMem(decideSound);
	DeleteSoundMem(cancelSound);
	DeleteSoundMem(selectSound);
}


