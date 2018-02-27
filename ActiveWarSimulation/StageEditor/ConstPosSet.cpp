#include"DxLib.h"
#include"ConstPosSet.h"
#include"EditActionSettings.h"
#include"EditAction.h"
#include"BattleObject.h"

//-------------ConstPosSet::ConstPosSetButton-------------
ConstPosSet::ConstPosSetButton::ConstPosSetButton(Vector2D point,Vector2D vec,int gridSetSize)
	:PosSettingButton(point,vec,std::to_string(gridSetSize)+"px"),m_gridSetSize(gridSetSize){}

void ConstPosSet::ConstPosSetButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pPosSetting=std::shared_ptr<PosSetting>(new ConstPosSet(m_point,m_vec,GetColor(255,255,0),m_gridSetSize));
}

//-------------ConstPosSet-------------
Vector2D ConstPosSet::CalculateClosestVertex(Vector2D point,Vector2D vec)const{
	int pointx=(int)point.x,pointy=(int)point.y;
	if(vec.x<0 && vec.y<0){
		//pointが左上にある場合
		return Vector2D((float)(pointx-pointx%m_gridSize),(float)(pointy-pointy%m_gridSize));
	}else if(vec.x>=0 && vec.y<0){
		//pointが右上にある場合
		return Vector2D((float)(pointx-pointx%m_gridSize+m_gridSize-1),(float)(pointy-pointy%m_gridSize));
	}else if(vec.x<0 && vec.y>=0){
		//pointが左下にある場合
		return Vector2D((float)(pointx-pointx%m_gridSize),(float)(pointy-pointy%m_gridSize+m_gridSize-1));
	}else{
		//pointが右下にある場合
		return Vector2D((float)(pointx-pointx%m_gridSize+m_gridSize-1),(float)(pointy-pointy%m_gridSize+m_gridSize-1));
	}
}

ConstPosSet::ConstPosSet(Vector2D buttonPos,Vector2D buttonSize,unsigned int buttonLightColor,int gridSize)
	:PosSetting(buttonPos,buttonSize,buttonLightColor),m_gridSize(max(gridSize,1)){}

void ConstPosSet::DrawGuide(int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY,Vector2D adjust)const{
	if(m_gridSize>=4){
		//グリッドの正方形の空洞の大きさが線2本分(=2px)以上になるのはm_gridSize>=4の時
		//表示されているマップの左上の座標をゲーム上の座標に置き換え、更にint化する
		int adjustx=(int)adjust.x,adjusty=(int)adjust.y;
		//左側縦線の描画
		for(int dx=adjustx%m_gridSize;dx<mapSizeX;dx+=m_gridSize){
			DrawLine(leftUpPosX+dx,leftUpPosY,leftUpPosX+dx,leftUpPosY+mapSizeY,GetColor(255,255,255));
		}
		//右側縦線の描画
		for(int dx=m_gridSize-adjustx%m_gridSize-1;dx<mapSizeX;dx+=m_gridSize){
			DrawLine(leftUpPosX+dx,leftUpPosY,leftUpPosX+dx,leftUpPosY+mapSizeY,GetColor(255,255,255));
		}
		//上側横線の描画
		for(int dy=adjusty%m_gridSize;dy<mapSizeY;dy+=m_gridSize){
			DrawLine(leftUpPosX,leftUpPosY+dy,leftUpPosX+mapSizeX,leftUpPosY+dy,GetColor(255,255,255));
		}
		//下側横線の描画
		for(int dy=m_gridSize-adjusty%m_gridSize-1;dy<mapSizeY;dy+=m_gridSize){
			DrawLine(leftUpPosX,leftUpPosY+dy,leftUpPosX+mapSizeX,leftUpPosY+dy,GetColor(255,255,255));
		}
	}
}

Vector2D ConstPosSet::CalculatePos(Vector2D point,const EditActionSettings &settings)const{
	int pointx=(int)point.x,pointy=(int)point.y;
	switch(settings.m_pEditAction->VGetPosSetKind(settings)){
	case(EditAction::NONEDIT):
		//そもそも位置調整をしない場合(例.BattleObjectのRemove対象選択時)
		return point;
	case(EditAction::BASENONEXIST):
		//基準点が存在しない場合(例.BattleObjectの設置位置決定時)
		//pointからm_gridSizeで割った時の余りを引いた座標を返す
		//settings.m_pBattleObject->getPos()-settings.m_pBattleObject->getResizeVec():図形が存在している方向を示す。
		return CalculateClosestVertex(point,settings.m_pBattleObject->getPos()-settings.m_pBattleObject->getResizeVec());
	case(EditAction::BASEEXIST):
		//基準点が存在する場合(例.BattleObjectのResize時)
		//pointが基準点よりどちら側にあるかで調整方法が異なる。同じ側のグリッド頂点の座標を返す。
		//point-settings.m_pBattleObject->getPos();//基準点からpointに向かうベクトル
		return CalculateClosestVertex(point,point-settings.m_pBattleObject->getPos());
	default:
		//例外処理
		return point;
	}
}
