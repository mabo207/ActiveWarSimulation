#include"ReadExplanation.h"
#include"input.h"
#include"DxLib.h"

//--------------ReflectionWork::ReadExplanation---------------
void ReflectionWork::ReadExplanation::Update(){
	//���ɍX�V���邱�Ƃ͂Ȃ�
}

bool ReflectionWork::ReadExplanation::WorkClear()const{
	return (keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1);
}

void ReflectionWork::ReadExplanation::WorkDraw()const{
	//���O���[�N�̗l�q��`��
	m_beforeWork->WorkDraw();
	//�}�`�ꗗ��`��
	for(const std::pair<std::shared_ptr<const Shape>,unsigned int> &info:m_assistShapeList){
		info.first->Draw(Vector2D(),info.second,TRUE,2.0f);
	}
}
