#include"GeneralPurposeResource.h"
#include"GraphicControl.h"
#include"DxLib.h"
#include"FilePath.h"

//---------------------GeneralPurposeResource---------------------
int GeneralPurposeResource::popLargeFont=-1;
int GeneralPurposeResource::cancelSound=-1;
int GeneralPurposeResource::decideSound=-1;
int GeneralPurposeResource::selectSound=-1;

void GeneralPurposeResource::LoadResource(){
	//�摜

	//�t�H���g
	popLargeFont=CreateFontToHandleEX("�肢�|�b�v�p R",60,7,DX_FONTTYPE_ANTIALIASING_EDGE_4X4);

	//��
	decideSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/decide.ogg").c_str());
	cancelSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/cancel.ogg").c_str());
	selectSound=LoadSoundMem((FilePath::effectSoundDir+"nonfree/select.ogg").c_str());
}

void GeneralPurposeResource::ReleaseResource(){
	//�摜

	//�t�H���g
	DeleteFontToHandleEX(popLargeFont);

	//��
	DeleteSoundMem(decideSound);
	DeleteSoundMem(cancelSound);
	DeleteSoundMem(selectSound);
}


