#include"DxLib.h"
#include"input.h"
#include"WatchLogScene.h"
#include"ToolsLib.h"
#include"FileRead.h"
#include"StringBuilder.h"
#include<map>
#include<optional>
#include"GraphicControl.h"
#include"SelectLogScene.h"
#include"FilePath.h"
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"
//ログ一覧
#include"AttackLog.h"
#include"WaitLog.h"
#include"InitLog.h"
#include"FinishLog.h"
//サブミッション
#include"ArcherAttackDistance.h"

//---------------------WatchLogScene-----------------
WatchLogScene::WatchLogScene(const std::string &logFileName)
	:m_logIndex(0),m_predictExplainFont(LoadFontDataToHandleEX(FilePath::fontDir+"PredictExplainFont.dft",1))
	,m_predictNumberFont(LoadFontDataToHandleEX(FilePath::fontDir+"PredictNumberFont.dft",3))
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
	//m_totalOPの計算
	m_totalOP=0.0f;
	const size_t logListSize=m_logList.size();
	for(size_t i=0;i+1<logListSize;i++){
		if((m_logList[i]->GetLogKind()==LogElement::LogKind::e_attack || m_logList[i]->GetLogKind()==LogElement::LogKind::e_wait)
			&& m_logList[i+1]->GetLogKind()!=LogElement::LogKind::e_finish
			&& m_logList[i]->m_unitDataList.size()>=2)
		{
			//BattleSceneDataと同様の、ターン数測定アルゴリズムを使用。次に行動するユニットのOPをmaxOPに増加させるのに必要な値だけm_totalOPを増加させる
			//最後の行動のOPは加算しないことに注意
			m_totalOP+=Unit::BattleStatus::maxOP-m_logList[i]->m_unitDataList[1].op;
		}
	}
	//m_survivalNumの計算
	m_survivalNum=0;
	if(logListSize>0 && m_logList[logListSize-1]->GetLogKind()==LogElement::LogKind::e_finish){
		for(const LogElement::UnitLogData &unitdata:m_logList[logListSize-1]->m_unitDataList){
			if(unitdata.punit->GetBattleStatus().team==Unit::Team::e_player){
				m_survivalNum++;
			} else{
				m_survivalNum--;
			}
		}
	}
	//サブミッションの評価情報を記録
	EvaluateAllLog();
	//最初のログをm_battleSceneDataに適用する
	m_logIndex=0;
	AdaptLog();
}

WatchLogScene::~WatchLogScene(){
	DeleteFontToHandleEX(m_predictExplainFont);
	DeleteFontToHandleEX(m_predictNumberFont);
}

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
	m_battleSceneData->DrawUnit(true,{m_battleSceneData->m_operateUnit});
	m_battleSceneData->m_operateUnit->DrawUnit(Vector2D(),0,true,true,true);
	m_battleSceneData->m_operateUnit->DrawMoveInfo();
	m_battleSceneData->m_operateUnit->DrawMaxMoveInfo();//どのくらい移動したのか分かりやすいようにするために描画
	//HPゲージの描画
	m_battleSceneData->DrawHPGage();
	//順番の描画
	m_battleSceneData->DrawOrder();
	//ダメージの描画
	if(m_logList[m_logIndex]->GetLogKind()==LogElement::LogKind::e_attack){
		//攻撃した場面の時は、どのような行動をしたかを見えるようにする
		const std::shared_ptr<AttackLog> attackLog=std::dynamic_pointer_cast<AttackLog>(m_logList[m_logIndex]);
		if(attackLog){
			const Vector2D pos=attackLog->GetAimedUnit()->getPos();
			attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->DrawPredict((int)pos.x,(int)pos.y,m_predictExplainFont,m_predictNumberFont,attackLog->GetOperateUnitData().punit,attackLog->GetAimedUnit());
		}
	}
	//クリアターン数と生存数を書く
	DrawStringRightJustifiedToHandle(CommonConstParameter::gameResolutionX-10,CommonConstParameter::gameResolutionY-130,"Total Turn : "+std::to_string((int)(m_totalOP/Unit::BattleStatus::maxOP)+1)+" Survival : "+std::to_string(m_survivalNum),GetColor(255,255,255),GeneralPurposeResource::gothicMiddleFont);
	//今のログの番号を書く
	DrawStringRightJustifiedToHandle(CommonConstParameter::gameResolutionX-10,CommonConstParameter::gameResolutionY-100,std::to_string(m_logIndex)+"/"+std::to_string(m_logList.size()),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	//サブミッション評価一覧を描画
	{
		std::string str;
		for(const std::pair<SubmissionEvaluation,size_t> &counter:m_evaluateCounter){
			if(counter.first!=SubmissionEvaluation::e_noevaluation){
				str+=(" "+counter.first.GetString()+":"+std::to_string(counter.second));
			}
		}
		DrawStringRightJustifiedToHandle(CommonConstParameter::gameResolutionX-10,CommonConstParameter::gameResolutionY-160,str,GetColor(255,255,255),GeneralPurposeResource::gothicMiddleFont);
	}
	//現在のログのサブミッション評価を書く
	DrawStringToHandle(0,0,m_evaluateList[m_logIndex].GetString().c_str(),GetColor(255,255,255),GeneralPurposeResource::gothicMiddleFont,m_evaluateList[m_logIndex].Color());
}

std::shared_ptr<GameScene> WatchLogScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//タイトル画面へ
	return CreateFadeOutInSceneCompletely(thisSharedPtr,std::make_shared<SelectLogScene::SelectLogSceneFactory>(),15,15);
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

void WatchLogScene::EvaluateAllLog(){
	//サブミッションの定義
	m_battleSceneData->m_scoreObserver->SetSubmissionRule(std::shared_ptr<SubmissionRuleBase>(new ArcherAttackDistance()));//ひとまず射手のを定義
	//すべてのログの評価
	for(size_t i=0,siz=m_logList.size();i<siz;i++){
		//m_battleSceneDataを該当ログの盤面にする
		m_logIndex=i;
		AdaptLog();
		//評価する
		if(m_battleSceneData->m_scoreObserver->GetSubmission().JudgeEvaluatedOrder(m_battleSceneData.get())){
			m_evaluateList.push_back(m_battleSceneData->m_scoreObserver->GetSubmission().GetRule()->RubricEvaluate(m_battleSceneData->m_field,m_battleSceneData->m_stageSize,m_logList[m_logIndex]));
		} else{
			m_evaluateList.push_back(SubmissionEvaluation::e_noevaluation);
		}
	}
	//ログの個数を集計する
	for(const SubmissionEvaluation &evaluate:m_evaluateList){
		auto it=m_evaluateCounter.find(evaluate);
		if(it==m_evaluateCounter.end()){
			//まだその評価がカウントされていない場合は、要素追加
			m_evaluateCounter.insert(std::make_pair(evaluate,1));
		} else{
			//すでにカウントしたことあるのなら、個数を+1する
			it->second++;
		}
	}
}
