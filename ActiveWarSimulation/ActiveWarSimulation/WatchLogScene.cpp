#include"DxLib.h"
#include"input.h"
#include"WatchLogScene.h"
#include"ToolsLib.h"
#include"FileRead.h"
#include"StringBuilder.h"
#include<map>
#include<optional>
#include"TitleScene.h"
//ログ一覧
#include"AttackLog.h"
#include"WaitLog.h"
#include"InitLog.h"
#include"FinishLog.h"

//---------------------WatchLogScene-----------------
WatchLogScene::WatchLogScene(const std::string &logFileName)
	:m_logIndex(0)
{
	//ファイルを読み込み、文字列分割
	const StringBuilder sb(FileStrRead(logFileName.c_str()),',','(',')');
	//解釈
	std::string stageDirName;
	StageLevel stageLevel;
	const StringBuilder *logSb=nullptr;
	for(const StringBuilder &ssb:sb.m_vec){
		if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="stageDirName"){
			stageDirName=ssb.m_vec[1].GetString();
		}
		if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="stageLevel"){
			stageLevel=StageLevel::CreateFromString(ssb.m_vec[1].GetString());
		}
		if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="log"){
			logSb=&ssb;
		}
	}
	//ステージ構築
	m_battleSceneData=std::shared_ptr<BattleSceneData>(new BattleSceneData(stageDirName,"",stageLevel));
	//ログ構築
	if(logSb!=nullptr){
		//ログにおけるindexとpunitの照合
		std::map<int,Unit *> indexToUnitPtr;
		for(const StringBuilder &ssb:logSb->m_vec[1].m_vec){
			if(!ssb.m_vec.empty() && ssb.m_vec[0].GetString()=="unitList"){
				for(size_t i=1,siz=ssb.m_vec.size();i<siz;i++){
					//ユニット情報の引き出し
					std::optional<int> index;
					std::optional<Vector2D> pos;
					for(const StringBuilder &sssb:ssb.m_vec[i].m_vec){
						if(sssb.m_vec.size()>=2 && sssb.m_vec[0].GetString()=="index"){
							index.emplace(std::strtol(sssb.m_vec[1].GetString().c_str(),nullptr,10));
						}
						if(sssb.m_vec.size()>=3 && sssb.m_vec[0].GetString()=="pos"){
							Vector2D p;
							p.x=std::strtof(sssb.m_vec[1].GetString().c_str(),nullptr);
							p.y=std::strtof(sssb.m_vec[2].GetString().c_str(),nullptr);
							pos.emplace(p);
						}
					}
					//該当ユニットを探す
					if(index.has_value() && pos.has_value()){
						for(Unit *pu:m_battleSceneData->m_unitList){
							if(pu->getPos()==pos.value()){
								indexToUnitPtr.insert(std::pair<int,Unit *>(index.value(),pu));
							}
						}
					}
				}
			}
		}
		//各ログの構築
		for(size_t logIndex=1,logSize=logSb->m_vec.size();logIndex<logSize;logIndex++){
			//ログの種類の決定と、全ての種類のログで共通のunitListの取り出しを行う
			std::string logKind;
			std::vector<LogElement::UnitLogData> unitList;
			for(const StringBuilder &ssb:logSb->m_vec[logIndex].m_vec){
				if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="kind"){
					logKind=ssb.m_vec[1].GetString();
				}
				if(!ssb.m_vec.empty() && ssb.m_vec[0].GetString()=="unitList"){
					for(size_t unitIndex=1,unitSize=ssb.m_vec.size();unitIndex<unitSize;unitIndex++){
						//要素を抽出
						std::optional<int> index;
						std::optional<int> hp;
						std::optional<float> op;
						std::optional<Vector2D> pos;
						for(const StringBuilder &dataSb:ssb.m_vec[unitIndex].m_vec){
							if(dataSb.m_vec.size()>=2 && dataSb.m_vec[0].GetString()=="index"){
								index.emplace(std::strtol(dataSb.m_vec[1].GetString().c_str(),nullptr,10));
							}
							if(dataSb.m_vec.size()>=2 && dataSb.m_vec[0].GetString()=="hp"){
								hp.emplace(std::strtol(dataSb.m_vec[1].GetString().c_str(),nullptr,10));
							}
							if(dataSb.m_vec.size()>=2 && dataSb.m_vec[0].GetString()=="op"){
								op.emplace(std::strtof(dataSb.m_vec[1].GetString().c_str(),nullptr));
							}
							if(dataSb.m_vec.size()>=3 && dataSb.m_vec[0].GetString()=="pos"){
								Vector2D p;
								p.x=std::strtof(dataSb.m_vec[1].GetString().c_str(),nullptr);
								p.y=std::strtof(dataSb.m_vec[2].GetString().c_str(),nullptr);
								pos.emplace(p);
							}
						}
						//unitListの格納
						if(index.has_value() && hp.has_value() && op.has_value() && pos.has_value()){
							unitList.push_back(LogElement::UnitLogData(hp.value(),op.value(),pos.value(),indexToUnitPtr.find(index.value())->second));
						}
					}
				}
			}
			//各ログの作成
			if(logKind=="AttackLog"){
				//attackLogのみ、狙われたユニットも引き出す
				const Unit *aimedUnit=nullptr;
				for(const StringBuilder &ssb:logSb->m_vec[logIndex].m_vec){
					if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="aimed"){
						aimedUnit=indexToUnitPtr.find(std::strtol(ssb.m_vec[1].GetString().c_str(),nullptr,10))->second;
					}
				}
				if(aimedUnit!=nullptr){
					m_logList.push_back(std::shared_ptr<LogElement>(new AttackLog(unitList,aimedUnit,std::vector<RouteInfo>{})));
				}
			} else if(logKind=="WaitLog"){
				m_logList.push_back(std::shared_ptr<LogElement>(new WaitLog(unitList,std::vector<RouteInfo>{})));
			} else if(logKind=="InitLog"){
				m_logList.push_back(std::shared_ptr<LogElement>(new InitLog(unitList)));
			} else if(logKind=="FinishLog"){
				m_logList.push_back(std::shared_ptr<LogElement>(new FinishLog(unitList,0)));
			}
		}
	}
	//ログをm_battleSceneDataに適用する
	AdaptLog();
}

WatchLogScene::~WatchLogScene(){}

void WatchLogScene::InitCompletely(){}

void WatchLogScene::Activate(){}

int WatchLogScene::Calculate(){
	//場面の選択
	if(m_logIndex>0 && (keyboard_get(KEY_INPUT_LEFT)==1 || (keyboard_get(KEY_INPUT_LEFT)>30 && keyboard_get(KEY_INPUT_LEFT)%5==0))){
		m_logIndex--;
		AdaptLog();
	} else if(m_logIndex+1<m_logList.size() && (keyboard_get(KEY_INPUT_RIGHT)==1 || (keyboard_get(KEY_INPUT_RIGHT)>30 && keyboard_get(KEY_INPUT_RIGHT)%5==0))){
		m_logIndex++;
		AdaptLog();
	}
	//遷移
	if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1){
		return 1;
	}

	return 0;
}

void WatchLogScene::Draw()const{
	//背景の描画
	m_battleSceneData->DrawField();
	//ユニットの描画
	m_battleSceneData->DrawUnit(true);
	//HPゲージの描画
	m_battleSceneData->DrawHPGage();
	//順番の描画
	m_battleSceneData->DrawOrder();
}

std::shared_ptr<GameScene> WatchLogScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//タイトル画面へ
	return CreateFadeOutInSceneCompletely(thisSharedPtr,std::make_shared<TitleScene::TitleSceneFactory>(),15,15);
}

void WatchLogScene::AdaptLog(){
	//操作ユニットの変更
	for(BattleObject *obj:m_battleSceneData->m_field){
		if(obj->GetType()==BattleObject::Type::e_unit && obj==m_logList[m_logIndex]->m_unitDataList.begin()->punit){
			Unit *pu=dynamic_cast<Unit *>(obj);
			if(pu!=nullptr){
				m_battleSceneData->m_operateUnit=pu;
			}
			break;
		}
	}
	//ユニットの状態を適用する
	for(BattleObject *ob:m_battleSceneData->m_field){
		if(ob->GetType()==BattleObject::Type::e_unit){
			Unit * const pu=dynamic_cast<Unit *>(ob);
			if(pu!=nullptr){
				//puがログに存在するかを調べる
				bool logExist=false;
				for(const LogElement::UnitLogData &data:m_logList[m_logIndex]->m_unitDataList){
					if(pu==data.punit){
						logExist=true;
						//存在した場合、情報適用する
						pu->SetFix(Shape::Fix::e_static);//生存しているのでstatic状態にする
						pu->SetOP(data.op);
						pu->AddHP(data.hp-pu->GetBattleStatus().HP);
						pu->Warp(data.pos);
						pu->SetPenetratable(m_battleSceneData->m_operateUnit->GetBattleStatus().team);
						break;
					}
				}
				if(!logExist){
					//ログが存在しなかった場合は、ユニットをマップ上に出さない
					pu->SetFix(Shape::Fix::e_ignore);
				}
			}
		}
	}
	//m_unitListの順番を整える
	m_battleSceneData->m_unitList.clear();
	for(const LogElement::UnitLogData &data:m_logList[m_logIndex]->m_unitDataList){
		//m_battleSceneData->m_unitList.push_back(data.punit);//本来はこうしたいけども、data.punitはconstなので格納できない
		//data.punitをm_battleSceneData->m_fieldから探す
		for(BattleObject *obj:m_battleSceneData->m_field){
			if(obj->GetType()==BattleObject::Type::e_unit && obj==data.punit){
				Unit *pu=dynamic_cast<Unit *>(obj);
				if(pu!=nullptr){
					m_battleSceneData->m_unitList.push_back(pu);
				}
				break;
			}
		}
	}
}
