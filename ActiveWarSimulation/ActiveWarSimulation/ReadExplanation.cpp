#include"ReadExplanation.h"
#include"input.h"
#include"DxLib.h"

//--------------ReflectionWork::ReadExplanation---------------
void ReflectionWork::ReadExplanation::AssistShapeInfo::Draw()const{
	int mode=0,pal=0;
	if(alpha!=opacityAlpha){
		//�����`�悷��ꍇ
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,alpha);
	}
	if(shape){
		shape->Draw(Vector2D(),color,TRUE,2.0f);
	}
	if(alpha!=opacityAlpha){
		//�����`�悵���ꍇ�̓u�����h���[�h�����ɖ߂�
		SetDrawBlendMode(mode,pal);
	}
}

void ReflectionWork::ReadExplanation::Update(){
	//���ɍX�V���邱�Ƃ͂Ȃ�
}

bool ReflectionWork::ReadExplanation::WorkClear()const{
	return (keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1);
}

void ReflectionWork::ReadExplanation::WorkDraw()const{
	//�}�`�ꗗ��`��
	for(const AssistShapeInfo &assistShape:m_assistShapeList){
		assistShape.Draw();
	}
}
