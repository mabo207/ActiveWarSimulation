#include"ReadExplanation.h"
#include"input.h"
#include"DxLib.h"

//--------------ReflectionWork::ReadExplanation---------------
void ReflectionWork::ReadExplanation::Update(){
	//特に更新することはない
}

bool ReflectionWork::ReadExplanation::WorkClear()const{
	return (keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1);
}

void ReflectionWork::ReadExplanation::WorkDraw()const{
	//直前ワークの様子を描画
	m_beforeWork->WorkDraw();
	//図形一覧を描画
	for(const std::pair<std::shared_ptr<const Shape>,unsigned int> &info:m_assistShapeList){
		info.first->Draw(Vector2D(),info.second,TRUE,2.0f);
	}
}
