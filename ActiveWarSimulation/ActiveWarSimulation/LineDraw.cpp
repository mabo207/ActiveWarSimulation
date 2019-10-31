#include"LineDraw.h"
#include"DxLib.h"
#include"input.h"
#include"ToolsLib.h"

namespace {
	const float acceptableDistance=5.0f;
	const float sqAcceptableDistance=acceptableDistance*acceptableDistance;
}

//---------------ReflectionWork::LineDraw--------------
ReflectionWork::LineDraw::LineDraw(const std::vector<Edge> &edgeList)
	:Base(Base::Kind::e_lineDraw)
	,m_inputing(false)
	,m_inputIndex(0)//テキトーで良い
	,m_inputStart(true)//テキトーで良い
{
	//m_lineListの初期化
	const size_t listSize=edgeList.size();
	m_lineList.reserve(listSize);
	for(size_t i=0;i<listSize;i++){
		m_lineList.push_back(edgeList[i]);
	}
}

ReflectionWork::LineDraw::~LineDraw(){}

void ReflectionWork::LineDraw::Update(){
	const Vector2D mouse=GetMousePointVector2D();
	const int left=mouse_get(MOUSE_INPUT_LEFT);
	if(m_inputing){
		//何らかの線を入力中
		//マウスを離したかの判定
		if(left==0){
			//目標点の計算
			Vector2D target;
			if(m_inputStart){
				//start始点であれば目標点はend
				target=m_lineList[m_inputIndex].edge.GetEndPoint();
			} else{
				//end始点であれば目標点はstart
				target=m_lineList[m_inputIndex].edge.GetBeginPoint();
			}
			//ちゃんと引けたかの判定とm_lineListの更新
			if((target-mouse).sqSize()<=sqAcceptableDistance){
				m_lineList[m_inputIndex].drawInfo=true;
			}
			//m_inputingの更新
			m_inputing=false;
		} else{
			//まだ入力中(何もしない)
		}
	} else{
		//線を探している
		if(left==1){
			//クリックした
			m_inputIndex=0;//初期化
			for(const size_t listSize=m_lineList.size();m_inputIndex<listSize;m_inputIndex++){
				//該当する線を探す
				if((m_lineList[m_inputIndex].edge.GetBeginPoint()-mouse).sqSize()<=sqAcceptableDistance){
					//始点を入力した
					m_inputing=true;
					m_inputStart=true;
					break;
				} else if((m_lineList[m_inputIndex].edge.GetEndPoint()-mouse).sqSize()<=sqAcceptableDistance){
					//終点を入力した
					m_inputing=true;
					m_inputStart=false;
					break;
				}
			}
		}
	}
}

bool ReflectionWork::LineDraw::WorkClear()const{
	//m_lineList[i].drawInfoが全てtrueである時だけtrueを返す
	for(const LineDrawInfo &line:m_lineList){
		if(!line.drawInfo){
			return false;
		}
	}
	return true;
}

void ReflectionWork::LineDraw::WorkDraw()const{
	for(const LineDrawInfo &info:m_lineList){
		if(info.drawInfo){
			//既に入力済みの直線は実線で
			info.edge.Shape::Draw(Vector2D(),GetColor(255,128,128),TRUE,5.0f);
		} else{
			//まだ入力していない直線は破線で
			info.edge.BrokenDraw(Vector2D(),GetColor(255,128,128),5.0f,8.0f,8.0f);
		}
	}
}
