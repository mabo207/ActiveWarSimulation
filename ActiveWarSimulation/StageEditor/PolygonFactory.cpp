#include"DxLib.h"
#include"PolygonFactory.h"
#include"MyPolygon.h"
#include"Edge.h"
#include"EditActionSettings.h"
#include"BattleObject.h"
#include"StageEditor.h"

//-----------------------PolygonFactory::PolygonFactoryButton-----------------------
PolygonFactory::PolygonFactoryButton::PolygonFactoryButton(Vector2D point,Vector2D vec)
	:ShapeFactoryButton(point,vec){}

void PolygonFactory::PolygonFactoryButton::ButtonDraw(int font,int fillFlag)const{
	//半透明化してボタンの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//内枠(ちょい透過させる)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//枠線の描画
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//外枠
	//当たり判定図形の描画
	const float size=20.0f;
	DrawTriangleAA((m_point+m_vec/2).x-size,(m_point+m_vec/2).y-size,(m_point+m_vec/2).x+size,(m_point+m_vec/2).y+size,(m_point+m_vec/2).x+size-5.0f,(m_point+m_vec/2).y+5.0f,GetColor(255,255,255),TRUE);
	DrawTriangleAA((m_point+m_vec/2).x-size,(m_point+m_vec/2).y-size,(m_point+m_vec/2).x+size,(m_point+m_vec/2).y+size,(m_point+m_vec/2).x+5.0f,(m_point+m_vec/2).y+size-5.0f,GetColor(255,255,255),TRUE);
}

void PolygonFactory::PolygonFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new PolygonFactory(m_point,m_vec,GetColor(255,255,0)));
	//現在選択しているオブジェクトも、当たり判定図形を変更する
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject->ChangeShape(settings.m_pShapeFactory->CreateShape(settings.m_pBattleObject->getPos()));
	}
}

//-----------------------PolygonFactory-----------------------
const float PolygonFactory::pointCapacity=5.0f;

PolygonFactory::PolygonFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor)
	:ShapeFactory(buttonPos,buttonSize,lightcolor){}

PolygonFactory::~PolygonFactory(){}

std::shared_ptr<Shape> PolygonFactory::CreateShape(Vector2D point)const{
	//ひとまず最初は線分を作る
	return std::shared_ptr<Shape>(new Edge(point,Vector2D(3.0f,3.0f),Shape::Fix::e_static));
}

EditPut::PosSetKind PolygonFactory::VPutAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings){
	if(pskind==EditPut::PosSetKind::BASENONEXIST){
		//置く場所を決めている時
		//位置を確定
		settings.m_pBattleObject->Warp(point);
		//頂点を初期化
		m_pointVec.clear();
		m_pointVec.push_back(point);
		//図形の頂点の決定へ
		return EditPut::PosSetKind::BASEEXIST;
	} else if(pskind==EditPut::PosSetKind::BASEEXIST){
		//図形の頂点を決めている時
		if((point-settings.m_pBattleObject->getPos()).sqSize()>pointCapacity*pointCapacity){
			//開始点よりマウスが遠くにある時、点を追加する
			bool flag=true;
			for(const Vector2D v:m_pointVec){
				if((v-point).sqSize()==0.0f){
					//いずれかの点と重なってたら点の追加は認めない
					flag=false;
					break;
				}
			}
			if(flag){
				//点の追加
				m_pointVec.push_back(point);
				//図形の変更
				if(m_pointVec.size()==2){
					//点が2つの時は線分になる
					settings.m_pBattleObject->ChangeShape(std::shared_ptr<Shape>(new Edge(m_pointVec[0],m_pointVec[1]-m_pointVec[0],settings.m_pBattleObject->GetFix())));
				} else if(m_pointVec.size()>=3){
					//点が3つ以上の時は多角形になる
					std::vector<Vector2D> noTop=m_pointVec;
					noTop.erase(noTop.begin());//先頭は別に用意するので除く
					settings.m_pBattleObject->ChangeShape(std::shared_ptr<Shape>(new MyPolygon(m_pointVec[0],noTop,settings.m_pBattleObject->GetFix())));
				}
			}
			return EditPut::PosSetKind::BASEEXIST;//図形の頂点の決定へ
		} else{
			//マウスが開始点の近くにある時に図形を確定する			
			settings.PutObject(settings.m_pBattleObject->getPos());
			settings.m_pBattleObject->ChangeShape(CreateShape(point));//多角形そのままにすると見づらいので線分に戻しておく
			m_pointVec.clear();//描画に影響が出るのでクリアしておく
			return EditPut::PosSetKind::BASENONEXIST;//図形の位置の決定へ
		}
	}
	return EditPut::PosSetKind::NONEDIT;//例外的な処理

}

void PolygonFactory::VPutNotPressAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings)const{
	if(pskind==EditPut::PosSetKind::BASENONEXIST){
		//置く場所を決めている時
		settings.m_pBattleObject.get()->Warp(point);//図形の位置を変える
	} else if(pskind==EditPut::PosSetKind::BASEEXIST){
		//置く図形の頂点を決めている時
		if(settings.m_pBattleObject->GetHitJudgeShape()->GetType()==Shape::Type::e_edge){
			//m_pBattleObjectが線分であれば(初期化の際は線分である)
			settings.m_pBattleObject->Resize(point);//図形の大きさを変える
		} else if(settings.m_pBattleObject->GetHitJudgeShape()->GetType()==Shape::Type::e_polygon){
			//多角形になっている時は特に何もしない
		}
	}
}

void PolygonFactory::FactoryDraw(const Vector2D adjust,const EditActionSettings &settings)const{
	//通常と同じく図形を描画
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	if(settings.GetMPOriginObject()!=nullptr){
		settings.GetMPOriginObject()->VDraw(adjust);
	}
	//通常と異なり、m_pointVecを全て結んだものを、現在の位置とともに表示
	const size_t size=m_pointVec.size();
	if(size>0){
		const unsigned int lineColor=GetColor(255,0,255);
		for(size_t i=0;i+1<size;i++){
			Vector2D p[2]={m_pointVec[i]+adjust,m_pointVec[i+1]+adjust};
			DrawLineAA(p[0].x,p[0].y,p[1].x,p[1].y,lineColor,1.0f);
		}
		const Vector2D mouse=GetMousePointVector2D();
		const Vector2D lastpoint=m_pointVec[size-1]+adjust;
		DrawLineAA(lastpoint.x,lastpoint.y,mouse.x,mouse.y,lineColor,1.0f);
		//始点にはcapacityの大きさの円を
		const Vector2D beginpoint=m_pointVec[0]+adjust;
		DrawCircleAA(beginpoint.x,beginpoint.y,pointCapacity,20,lineColor,TRUE);
	}
	//設定を元に戻す
	SetDrawBlendMode(mode,pal);
}
