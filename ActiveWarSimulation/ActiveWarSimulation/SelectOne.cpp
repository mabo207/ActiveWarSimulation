#include"SelectOne.h"
#include"input.h"
#include"DxLib.h"

//-----------------ReflectionWork::SelectOne-----------------
ReflectionWork::SelectOne::SelectOne(const std::shared_ptr<const Shape> &answer,const std::vector<std::shared_ptr<const Shape>> &dummyList,const std::string &question)
	:Base(Kind::e_selectOne,question)
	,m_selected(false)
	,m_correct(false)
{
	//m_choiceListÇÃç\ê¨
	m_choiceList.push_back(Component(answer,true));
	for(const std::shared_ptr<const Shape> &shape:dummyList){
		m_choiceList.push_back(Component(shape,false));
	}
}

ReflectionWork::SelectOne::~SelectOne(){}

void ReflectionWork::SelectOne::Update(){
	if(!m_selected && mouse_get(MOUSE_INPUT_LEFT)==1){
		const Vector2D mouse=GetMousePointVector2D();
		for(const Component &choice:m_choiceList){
			if(choice.shape->VJudgePointInsideShape(mouse)){
				m_selected=true;
				m_correct=choice.correct;
				break;
			}
		}
	}
}

bool ReflectionWork::SelectOne::WorkClear()const{
	return m_selected;
}

void ReflectionWork::SelectOne::WorkDraw()const{
	const Vector2D mouse=GetMousePointVector2D();
	for(const Component &choice:m_choiceList){
		if(choice.shape->VJudgePointInsideShape(mouse)){
			//ëIëéàÇ…èdÇ»Ç¡ÇΩÇÁògÇï`âÊ
			choice.shape->Draw(Vector2D(),GetColor(255,255,0),FALSE,2.0f);
			break;
		}
	}
}
