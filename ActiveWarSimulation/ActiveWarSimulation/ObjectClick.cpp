#include"DxLib.h"
#include"ObjectClick.h"
#include"ToolsLib.h"
#include"input.h"

//--------------ReflectionWork::ObjectClick---------------
ReflectionWork::ObjectClick::ObjectClick(const std::vector<std::shared_ptr<const Shape>> &shapeList,const std::string &question)
	:Base(Kind::e_clickObject,question)
{
	for(const std::shared_ptr<const Shape> &shape:shapeList){
		m_objectList.push_back(ObjectInfo(shape));
	}
}

ReflectionWork::ObjectClick::~ObjectClick(){}

void ReflectionWork::ObjectClick::Update(){
	if(mouse_get(MOUSE_INPUT_LEFT)==1){
		const Vector2D mouse=GetMousePointVector2D();
		for(ObjectInfo &info:m_objectList){
			if(!info.clicked && info.shape->VJudgePointInsideShape(mouse)){
				//まだクリックされたことのない図形でクリックされたら
				info.clicked=true;
				break;
			}
		}
	}
}

bool ReflectionWork::ObjectClick::WorkClear()const{
	//3個のオブジェクトがクリックされたらワークが終わったことにする
	size_t count=0;
	bool allClick=true;//オブジェクトが3個未満の時の対策
	for(const ObjectInfo &info:m_objectList){
		if(info.clicked){
			count++;
		} else{
			allClick=false;
		}
	}
	return count>=3 || allClick;
}

void ReflectionWork::ObjectClick::WorkDraw()const{
	for(const ObjectInfo &info:m_objectList){
		if(info.clicked){
			//クリックされているオブジェクトは半透明塗りつぶし
			int mode,pal;
			GetDrawBlendMode(&mode,&pal);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
			info.shape->Draw(Vector2D(),GetColor(255,128,128),TRUE,0.0f);
			SetDrawBlendMode(mode,pal);
		} else{
			//クリックされていないオブジェクトは描画しない
			//info.shape->Draw(Vector2D(),GetColor(255,128,128),FALSE,5.0f);
		}
	}
	//マウスが合っている図形を黄色で目立たせる
	const Vector2D mouse=GetMousePointVector2D();
	for(const ObjectInfo &info:m_objectList){
		if(!info.clicked && info.shape->VJudgePointInsideShape(mouse)){
			info.shape->Draw(Vector2D(),GetColor(255,255,128),FALSE,2.0f);
			break;
		}
	}
}
