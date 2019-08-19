#include<math.h>
#include"EditActionSettings.h"

#include"DxLib.h"
#include"EditAction.h"
#include"ShapeFactory.h"
#include"PosSetting.h"
#include"SelectLevel.h"
#include"StringBuilder.h"
#include"FileRead.h"
#include"CommonConstParameter.h"

#include"Terrain.h"
#include"Unit.h"
#include"Circle.h"

EditActionSettings::EditActionSettings(std::shared_ptr<EditAction> pEditAction, std::shared_ptr<BattleObject> pBattleObject,std::shared_ptr<ShapeFactory> pShapeFactory,std::shared_ptr<PosSetting> pPosSetting)
	:m_adjust(0,0),m_pEditAction(pEditAction),m_pBattleObject(pBattleObject),m_pShapeFactory(pShapeFactory),m_pPosSetting(pPosSetting),m_pOriginObject(std::shared_ptr<BattleObject>(nullptr))
	,m_mouseObjectDepth(0,Vector2D(0.0f,0.0f))
{
	
}

EditActionSettings::~EditActionSettings() {}

std::vector<std::shared_ptr<BattleObject>>::const_iterator EditActionSettings::GetMousePointedObject(Vector2D point)const{
	std::vector<std::shared_ptr<BattleObject>>::const_iterator it=m_objects.begin(),ite=m_objects.end();
	size_t count=0;
	for(;it!=ite;it++) {
		//マウスが被っている図形には枠を描画しフォーカスを表現。ただしm_mouseobjectDepth.firstコ目に見つけたもののみ
		if(it->get()->JudgePointInsideShape(point)){
			if(count==m_mouseObjectDepth.first){
				break;
			} else{
				//個数に達していない場合はそのイテレータを返さない
				count++;
			}
		}
	}
	return it;
}

const BattleObject *EditActionSettings::GetMousePointedObjectPointer(Vector2D point)const{
	std::vector<std::shared_ptr<BattleObject>>::const_iterator it=GetMousePointedObject(point);
	if(it==m_objects.end()){
		return nullptr;
	}
	return it->get();
}

void EditActionSettings::PracticeEdit(Vector2D point){
	//マウス座標をマップ実座標に変換し、更に位置調整してから編集
	m_pEditAction.get()->VProcessAction(m_pPosSetting->CalculatePos(point+m_adjust,*this),*this);
}

void EditActionSettings::PracticeNonPressEdit(Vector2D point){
	//マウス座標をマップ実座標に変換し、更に位置調整してから編集
	m_pEditAction.get()->VNonPressEditing(m_pPosSetting->CalculatePos(point+m_adjust,*this),*this);
}

void EditActionSettings::PushScrollBar(float scrollpx,float maxX,float maxY,int mouseX,int mouseY,int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY){
	if(mouseY*leftUpPosX-mouseX*leftUpPosY<0 && (mouseY-leftUpPosY)*leftUpPosX+(mouseX-(mapSizeX+leftUpPosX))*leftUpPosY<0){
		//上部分(1より下、2より下)
		m_adjust.y=fmax((float)0,m_adjust.y-scrollpx);
	} else if(mouseY*leftUpPosX-mouseX*leftUpPosY>=0 && (mouseY-(leftUpPosY+mapSizeY))*leftUpPosX+(mouseX-leftUpPosX)*leftUpPosY<0){
		//左部分(1より上、2より下)
		m_adjust.x=fmax((float)0,m_adjust.x-scrollpx);
	} else if((mouseY-leftUpPosY)*leftUpPosX+(mouseX-(mapSizeX+leftUpPosX))*leftUpPosY>=0 && (mouseY-(leftUpPosY+mapSizeY))*leftUpPosX-(mouseX-(mapSizeX+leftUpPosX))*leftUpPosY<0){
		//右部分(1より下、2より上)
		m_adjust.x=fmin(maxX-mapSizeX,m_adjust.x+scrollpx);
	} else{
		//下部分(1より上、2より上)
		m_adjust.y=fmin(maxY-mapSizeY,m_adjust.y+scrollpx);
	}
}

void EditActionSettings::PushScrollBar(Vector2D move){
	//最大値最小値を考慮する
	Vector2D a=m_adjust+move;
	float maxX=2000-(float)CommonConstParameter::mapSizeX,maxY=2000-(float)CommonConstParameter::mapSizeY;
	m_adjust=Vector2D(
		a.x>=0 ? (a.x<maxX ? a.x : maxX) : 0
		,a.y>=0 ? (a.y<maxY ? a.y : maxY) : 0
	);
}

void EditActionSettings::DrawEditButtonPushed()const{
	m_pEditAction.get()->DrawPushedButton();
}

void EditActionSettings::DrawShapeFactoryButtonPushed()const{
	m_pShapeFactory->DrawPushedButtonLight();
}

void EditActionSettings::DrawPosSettingButtonPushed()const{
	m_pPosSetting->DrawPushedButtonLight();
}

void EditActionSettings::DrawPosSettingGuide(int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY)const{
	m_pPosSetting->DrawGuide(leftUpPosX,leftUpPosY,mapSizeX,mapSizeY,m_adjust);
}

void EditActionSettings::PutObject(Vector2D point){
	if(m_pBattleObject.get()!=nullptr){
		//オブジェクトを現在のマウスの位置に合わせてから設置
		m_pBattleObject.get()->Warp(point);
		m_objects.push_back(m_pBattleObject);
		//m_pBattleObjectをそのままにすると同じポインタのオブジェクトを違う場所に置こうとしてしまうので、ポインタは新しくする
		m_pBattleObject=m_pBattleObject->VCopy();
	}
}

void EditActionSettings::RemoveObject(Vector2D point){
	//取り除くオブジェクトを探す
	std::vector<std::shared_ptr<BattleObject>>::const_iterator it=GetMousePointedObject(point);
	if(it!=m_objects.end()){
		m_objects.erase(it);
	}
}

void EditActionSettings::ReplaceBattleObject(const std::shared_ptr<BattleObject> &obj){
	//取り除くオブジェクトを探す
	for(std::vector<std::shared_ptr<BattleObject>>::iterator it=m_objects.begin(),ite=m_objects.end();it!=ite;it++){
		if(*it==m_pBattleObject){
			*it=obj;//置き換える
			break;
		}
	}
}

void EditActionSettings::SetEditObject(Vector2D point){
	std::vector<std::shared_ptr<BattleObject>>::const_iterator it=GetMousePointedObject(point);
	if(it!=m_objects.end()){
		m_pBattleObject=*it;
		m_pOriginObject=it->get()->VCopy();
	}
}

void EditActionSettings::CancelEditing(){
	//エディタで変更できるのは位置と大きさのみ
	if(m_pBattleObject.get()!=nullptr && m_pOriginObject.get()!=nullptr){
		m_pBattleObject.get()->Warp(m_pOriginObject.get()->getPos());
		m_pBattleObject.get()->Resize(m_pOriginObject.get()->getResizeVec());
	}
}

void EditActionSettings::InitEditObject(){
	m_pBattleObject=std::shared_ptr<BattleObject>(nullptr);
	m_pOriginObject=std::shared_ptr<BattleObject>(nullptr);
}

void EditActionSettings::UpdateMouseObjectDepth(const int keyinputright){
	const Vector2D vec=GetMousePointVector2D();//マウスの位置はマップ基準である必要はない
	if(keyinputright==1){
		//右クリックで回数を増やす
		m_mouseObjectDepth.first++;
		m_mouseObjectDepth.second=vec;
	}else if((vec-m_mouseObjectDepth.second).sqSize()>=100.0f){
		//前に右クリックした位置から10px以上離れていれば
		m_mouseObjectDepth.first=0;
		m_mouseObjectDepth.second=vec;
	}
}

//m_objectsの初期化
void EditActionSettings::InitObjects(){
	m_objects.clear();
}

//制作データの書き出し
void EditActionSettings::WriteOutStage(const char *filename)const{
	/*
	形式例
	{(オブジェクト情報),(図形情報)}
	{(Terrain),(Edge,(200,20),(30,10))}
	*/
	//ファイルを開く
	std::ofstream ofs(filename,std::ios_base::trunc);
	if(!ofs){
		return;
	}
	//全ての非Unitオブジェクト情報を書き出し
	for(const std::shared_ptr<BattleObject> &pObj:m_objects){
		if(pObj->GetType()!=BattleObject::Type::e_unit){
			pObj->WriteOutObjectWholeInfo(ofs);
		}
	}
	//終了処理
	ofs.close();
}

//ユニットの書き出し
void EditActionSettings::WriteOutUnit()const{
	/*
	後で手編集してもいいので、最低限の体裁を整える
	形式例
	{(定義方法),(名前),(職業),(レベル),(位置),(チーム分け),(AI系統),(その他(無視内容)：初期化HPなど)}
	{(definition,mob),(name,敵兵),(profession,0),(lv,1),(pos,470,600),(team,1),(ai,1,-1),(initHP,5)}
	*/
	//ファイルを開く
	std::ofstream ofs(m_pSelectLevel->GetUnitListFileName().c_str(),std::ios_base::trunc);
	if(!ofs){
		return;
	}
	//全てのユニット情報を書き出し
	for(const std::shared_ptr<BattleObject> &pObj:m_objects){
		if(pObj->GetType()==BattleObject::Type::e_unit){
			//definitionがmob一択なので、Unitクラス内でなくここに処理を書く。
			const std::shared_ptr<Unit> pUnit=std::dynamic_pointer_cast<Unit>(pObj);
			if(pUnit){
				const Unit::BattleStatus battleStatus=pUnit->GetBattleStatus();
				const Unit::BaseStatus baseStatus=pUnit->GetBaseStatus();
				const Vector2D pos=pUnit->getPos();
				ofs<<"{(definition,mob),";
				ofs<<"(name,"<<baseStatus.name<<"),";
				ofs<<"(profession,"<<baseStatus.profession<<"),";
				ofs<<"(lv,"<<baseStatus.lv<<"),";
				ofs<<"(weapon,"<<battleStatus.weapon->GetResisterName()<<"),";
				ofs<<"(pos,"<<(int)(pos.x)<<','<<(int)(pos.y)<<"),";
				ofs<<"(team,"<<battleStatus.team<<"),";
				ofs<<"(ai,"<<battleStatus.aitype<<','<<battleStatus.aiGroup<<')';
				if(battleStatus.HP<baseStatus.maxHP){
					//HPが減っている処理をしたなら、initHPの項目を追加
					ofs<<",(initHP,"<<battleStatus.HP<<')';
				}
				ofs<<'}'<<std::endl;
			}
		}
	}
	//終了処理
	ofs.close();
}

//ステージの読み込み
void EditActionSettings::ReadStage(const char *filename){
	//オブジェクト群は{}で囲まれ\nで区切られているので、１階層だけ分割読み込みして、オブジェクトを生成する
	try{
		StringBuilder sb(FileStrRead(filename),'\n','{','}');
		for(StringBuilder &ssb:sb.m_vec){
			const std::shared_ptr<BattleObject> pb=BattleObject::CreateObject(ssb);//sb,ssbは変更される
			if(pb.get()!=nullptr){
				m_objects.push_back(pb);
			}
		}
	} catch(const FileOpenFailedException &){
		//ファイル読み込み失敗を許容する
	}
}

//ステージの読み込み
void EditActionSettings::ReadUnit(){
	try{
		StringBuilder unitlist(FileStrRead(m_pSelectLevel->GetUnitListFileName().c_str()),'\n','{','}');
		for(StringBuilder &unitdata:unitlist.m_vec){
			const std::shared_ptr<BattleObject> punit(Unit::CreateUnitFromBuilder(unitdata));
			if(punit){
				m_objects.push_back(punit);
			}
		}
	} catch(const FileOpenFailedException &){
		//ファイル読み込み失敗を許容する
	}
}

//データの読み込み
void EditActionSettings::ReadData(){
	InitObjects();
	ReadStage("SaveData/stage.txt");
	ReadUnit();
}

//兵種から推奨武器種を返す
Weapon::Kind EditActionSettings::ProfessionToWeaponKind(Unit::Profession::Kind profession){
	Weapon::Kind weaponKind;
	switch(profession){
	case(Unit::Profession::e_soldier):
		weaponKind=Weapon::Kind::e_sword;
		break;
	case(Unit::Profession::e_armer):
		weaponKind=Weapon::Kind::e_lance;
		break;
	case(Unit::Profession::e_archer):
		weaponKind=Weapon::Kind::e_bow;
		break;
	case(Unit::Profession::e_mage):
		weaponKind=Weapon::Kind::e_book;
		break;
	case(Unit::Profession::e_healer):
		weaponKind=Weapon::Kind::e_rod;
		break;
	}
	return weaponKind;
}
