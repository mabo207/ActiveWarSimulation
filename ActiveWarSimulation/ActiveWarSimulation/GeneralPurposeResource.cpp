#include"GeneralPurposeResource.h"
#include"DxLib.h"
#include"FilePath.h"

//---------------------GeneralPurposeResource---------------------
int GeneralPurposeResource::cancelSound=-1;
int GeneralPurposeResource::decideSound=-1;
int GeneralPurposeResource::selectSound=-1;

void GeneralPurposeResource::LoadResource(){
	//�摜

	//�t�H���g

	//��
	decideSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/decide.ogg").c_str());
	cancelSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/cancel.ogg").c_str());
	selectSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/select.ogg").c_str());
}

void GeneralPurposeResource::ReleaseResource(){
	//�摜

	//�t�H���g

	//��
	DeleteSoundMem(decideSound);
	DeleteSoundMem(cancelSound);
	DeleteSoundMem(selectSound);
}


