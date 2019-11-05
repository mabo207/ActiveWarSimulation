#include"DxLib.h"
#include"ObjectClick.h"
#include"ToolsLib.h"
#include"input.h"

//--------------ReflectionWork::ObjectClick---------------
ReflectionWork::ObjectClick::ObjectClick(const std::vector<std::shared_ptr<const Shape>> &shapeList)
	:Base(Kind::e_clickObject)
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
	//全てのオブジェクトがクリックされたらワークが終わったことにする
	for(const ObjectInfo &info:m_objectList){
		if(!info.clicked){
			return false;
		}
	}
	return true;
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
			//クリックされていないオブジェクトは枠線のみ描画
			info.shape->Draw(Vector2D(),GetColor(255,128,128),FALSE,5.0f);
		}
	}
}
