#include"GeneralPurposeResource.h"
#include"GraphicControl.h"
#include"DxLib.h"
#include"FilePath.h"

//---------------------GeneralPurposeResource---------------------
int GeneralPurposeResource::popLargeFont=-1;
int GeneralPurposeResource::gothicMiddleFont=-1;
int GeneralPurposeResource::cancelSound=-1;
int GeneralPurposeResource::decideSound=-1;
int GeneralPurposeResource::selectSound=-1;

void GeneralPurposeResource::LoadResource(){
	//�摜

	//�t�H���g
	popLargeFont=LoadFontDataToHandleEX(FilePath::fontDir+"LargePopFont.dft",0);
	gothicMiddleFont=LoadFontDataToHandleEX(FilePath::fontDir+"ExplainGothicFont.dft",0);

	//��
	decideSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/decide.ogg").c_str());
	cancelSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/cancel.ogg").c_str());
	selectSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/select.ogg").c_str());
}

void GeneralPurposeResource::ReleaseResource(){
	//�摜

	//�t�H���g
	DeleteFontToHandleEX(popLargeFont);
	DeleteFontToHandleEX(gothicMiddleFont);

	//��
	DeleteSoundMem(decideSound);
	DeleteSoundMem(cancelSound);
	DeleteSoundMem(selectSound);
}


