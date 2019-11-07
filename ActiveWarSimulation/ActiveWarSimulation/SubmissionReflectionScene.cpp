#define _USE_MATH_DEFINES
#include"DxLib.h"
#include"input.h"
#include"SubmissionReflectionScene.h"
#include"StageClearScene.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"
#include"LogElement.h"
#include"GeneralPurposeResource.h"
#include"GraphicControl.h"
#include"FilePath.h"
#include"WaitLog.h"
#include"AttackLog.h"
//リフレクション活動一覧
#include"LineDraw.h"
#include"ObjectClick.h"
//仮想図形構築のために必要なもの
#include"Edge.h"
#include"MyPolygon.h"
#include<cmath>

namespace {
	const int lineWidth=5;
	const unsigned int merginColor=GetColor(128,128,255);
	const float minimapRate=0.5f;
	const int minimapWidth=(int)(CommonConstParameter::mapSizeX*minimapRate);
	const int minimapHeight=(int)(CommonConstParameter::mapSizeY*minimapRate);
	const Vector2D minimapPos[2]={Vector2D(20.0f,300.0f),Vector2D(980.0f,300.0f)};
	const int minimapX[2]={(int)minimapPos[0].x,(int)minimapPos[1].x};
	const int minimapY[2]={(int)minimapPos[0].y,(int)minimapPos[1].y};
}
//---------------SubmissionReflectionScene::MinimapDrawInfo-----------------
SubmissionReflectionScene::MinimapDrawInfo::MinimapDrawInfo(const std::shared_ptr<const LogElement> &log,Unit::Team::Kind phase)
	:pOperateUnit(nullptr),pAttackedUnit(nullptr)
{
	if(log){
		//logの有効性判定
		//関数の作成
		const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(log);
		const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(log);
		std::function<const Unit *()> getOperateUnit,getAttackedUnit;
		if(log->GetLogKind()==LogElement::LogKind::e_wait && waitLog){
			//待機ログの場合
			getOperateUnit=[&waitLog](){return waitLog->GetOperateUnitData().punit;};
		} else if(log->GetLogKind()==LogElement::LogKind::e_attack && attackLog){
			//攻撃ログの場合
			getOperateUnit=[&attackLog](){return attackLog->GetOperateUnitData().punit;};
			getAttackedUnit=[&attackLog](){return attackLog->GetAimedUnit();};
		}
		//盤面のユニット一覧の作成
		const size_t maxSize=log->m_unitDataList.size();
		size_t operateIndex=maxSize,attackedIndex=maxSize,index=0;//pOperateUnitとpAttackedUnitの探索に用いる
		for(const LogElement::UnitLogData &logData:log->m_unitDataList){
			//ユニットデータのコピー（加工をするため）
			Unit u=*logData.punit;
			//ユニットデータの加工
			u.Warp(logData.pos);
			u.AddHP(logData.hp-u.GetBattleStatus().HP);
			u.SetOP(logData.op);
			u.SetPenetratable(phase);
			//operateIndex,attackedIndexの探索、関数が存在するかどうか気をつける
			if(getOperateUnit && logData.punit==getOperateUnit()){
				operateIndex=index;
				u.SetPenetratable(u.GetBattleStatus().team);//行動ユニットについては、侵入不可範囲を常に広げない状態にする（見づらいため）
			}
			if(getAttackedUnit && logData.punit==getAttackedUnit()){
				attackedIndex=index;
			}
			//格納
			unitList.push_back(u);
			//探索用indexの更新
			index++;
		}
		//pOperateUnitとpAttackedUnitの探索(これ以降unitListの操作は行われない)
		if(operateIndex<maxSize){
			pOperateUnit=&unitList[operateIndex];
		}
		if(attackedIndex<maxSize){
			pAttackedUnit=&unitList[attackedIndex];
		}
	}
}

SubmissionReflectionScene::MinimapDrawInfo::~MinimapDrawInfo(){}

//---------------SubmissionReflectionScene-----------------
SubmissionReflectionScene::SubmissionReflectionScene(const std::shared_ptr<BattleSceneData> &battleSceneData,const std::shared_ptr<BattleSceneElement> &clearScene)
	:BattleSceneElement(SceneKind::e_submissionReflection)
	,m_battleSceneData(battleSceneData)
	,m_clearScene(clearScene)
	,m_operateCursor(LoadGraphEX(FilePath::graphicDir+"operatedCursor.png"))
	,m_predictNumberFont(CreateFontToHandleEX("メイリオ",28,8,DX_FONTTYPE_EDGE))
	,m_predictExplainFont(CreateFontToHandleEX("メイリオ",10,2,DX_FONTTYPE_EDGE))
{
	//m_goodLogInfo,m_badLogInfoの初期化
	const WholeReflectionInfo reflectionInfo=m_battleSceneData->m_scoreObserver->GetSubmission().GetReflectionInfo();
	//射手の安全地帯についてのワークなので、敵フェーズの侵入不可範囲設定にする
	const Unit::Team::Kind phaseSetting=Unit::Team::e_enemy;//サブミッションによってここのフェーズ設定が決められる
	m_goodLogInfo.emplace(reflectionInfo.m_goodLog.second,phaseSetting);
	m_badLogInfo.emplace(reflectionInfo.m_badLog.second,phaseSetting);
	//m_reflectionWorkの初期化
	InitReflectionWork();
}

SubmissionReflectionScene::~SubmissionReflectionScene(){
	DeleteGraphEX(m_operateCursor);
	DeleteFontToHandleEX(m_predictNumberFont);
	DeleteFontToHandleEX(m_predictExplainFont);
}

void SubmissionReflectionScene::DrawResizedMap(int x,int y,const MinimapDrawInfo &minimapInfo)const{
	//マージンの描画
	DrawBox(x-lineWidth,y-lineWidth,x+minimapWidth+lineWidth,y+minimapHeight+lineWidth,merginColor,TRUE);
	const Vector2D startPos((float)x,(float)y);
	//BattleSceneDataの描画関数は拡大縮小描画に対応していないので、独自に実装する
	//背景描画
	DrawExtendGraphExRateAssign(startPos.x,startPos.y,minimapRate,m_battleSceneData->m_mapPic,TRUE);
	//ユニットの描画
	for(size_t i=1,siz=minimapInfo.GetUnitList().size();i<siz+1;i++){
		const size_t index=i%siz;
		//ウインドウに入っていない物は描画しない
		//当たり判定図形のm_fix判定は行わない、そもそも死んでいるかどうかの判定なので、「実際のマップでは死んでいるが仮想マップでは生きているようにしたい」という要望には対応できないから
		//先頭ユニットは最後に描画する
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&minimapInfo.GetUnitList()[index])){
			//アクション範囲の描画フラグ
			bool actionRangeDraw,animationFlag;
			if(index==0){
				//先頭ユニットのだけ描画フラグを立てる
				actionRangeDraw=true;
				animationFlag=true;
			} else{
				actionRangeDraw=false;
				animationFlag=false;
			}
			//退却したユニット(m_fixがe_ignore)は描画しない
			minimapInfo.GetUnitList()[index].DrawUnit(minimapInfo.GetUnitList()[index].getPos(),startPos,minimapRate,0,animationFlag,true,actionRangeDraw);
		}
	}
	//ユニットのHPゲージの描画
	for(size_t i=1,siz=minimapInfo.GetUnitList().size();i<siz+1;i++){
		const size_t index=i%siz;
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&minimapInfo.GetUnitList()[index])){
			//ウインドウに入っていない物は描画しない
			minimapInfo.GetUnitList()[index].DrawHPGage(minimapInfo.GetUnitList()[index].getPos(),startPos,minimapRate);
		}
	}
	//操作ユニットのマーカーを表示
	if(minimapInfo.pOperateUnit!=nullptr){
		float dx,dy;
		GetGraphSizeF(m_operateCursor,&dx,&dy);
		const Vector2D pos=(minimapInfo.pOperateUnit->getPos()+Vector2D(0.0f,-Unit::unitCircleSize+10.0f))*minimapRate+startPos-Vector2D(dx/2.0f,dy);
		DrawGraph((int)pos.x,(int)pos.y,m_operateCursor,TRUE);
	}
	//ダメージの表示
	if(minimapInfo.pOperateUnit!=nullptr && minimapInfo.pAttackedUnit!=nullptr){
		//攻撃時は、攻撃相手の上にダメージを表示
		const Vector2D pos=minimapInfo.pAttackedUnit->getPos()*minimapRate+startPos;
		minimapInfo.pOperateUnit->GetBattleStatus().weapon->DrawPredict((int)pos.x,(int)pos.y,m_predictExplainFont,m_predictNumberFont,minimapInfo.pOperateUnit,minimapInfo.pAttackedUnit);
	}
}

int SubmissionReflectionScene::thisCalculate(){
	if(m_reflectionWork){
		//ワーク入力処理
		m_reflectionWork->Update();
		//ワーク切り替え処理
		if(m_reflectionWork->WorkClear()){
			//ワークをクリアしたら
			m_reflectionWork.reset();
		}
	} else{
		//ワークが設定されていない時に、遷移処理を行う
		if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
			return SceneKind::e_clear;
		}
	}

	return SceneKind::e_submissionReflection;
}

void SubmissionReflectionScene::thisDraw()const{
	//文章の描画
	DrawStringToHandle(30,30,"どちらの方が良いとされる行動でしょうか？",GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	//比較マップの描画
	if(m_goodLogInfo.has_value()){
		DrawResizedMap(minimapX[0],minimapY[0],m_goodLogInfo.value());
	}
	if(m_badLogInfo.has_value()){
		DrawResizedMap(minimapX[1],minimapY[1],m_badLogInfo.value());
	}
	//ワークについての描画
	if(m_reflectionWork){
		m_reflectionWork->WorkDraw();
	}
}

int SubmissionReflectionScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_clear):
		m_nextScene=m_clearScene;
		m_clearScene.reset();//m_nextSceneにコピーしたので、所有権を放棄
		break;
	}
	return index;
}

void SubmissionReflectionScene::ReturnProcess(){
	//特に何もしない
}

void SubmissionReflectionScene::InitReflectionWork(){
	//ワークの初期化（暫定）
/*
	//攻撃ユニットと被攻撃ユニットを結ぶ線を引くワーク
	const Vector2D goodLogStart=minimapPos[0]+m_goodLogInfo->pOperateUnit->getPos()*minimapRate;
	const Vector2D goodLogEnd=minimapPos[0]+m_goodLogInfo->pAttackedUnit->getPos()*minimapRate;
	const Vector2D badLogStart=minimapPos[1]+m_badLogInfo->pOperateUnit->getPos()*minimapRate;
	const Vector2D badLogEnd=minimapPos[1]+m_badLogInfo->pAttackedUnit->getPos()*minimapRate;
	m_reflectionWork=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::LineDraw(
		{Edge(goodLogStart,goodLogEnd-goodLogStart,Shape::Fix::e_ignore)
		,Edge(badLogStart,badLogEnd-badLogStart,Shape::Fix::e_ignore)}
	));
//*/
	//攻撃ユニットと被攻撃ユニットを結ぶ線上の障害物をクリックするワーク
	std::vector<std::shared_ptr<const Shape>> shapeList;
	const auto addFunc=[&shapeList,this](std::shared_ptr<Shape> &addShape,const Vector2D &minimapPosition,const Shape *conditionShape){
		//地図に合うように加工
		const Vector2D pos=addShape->GetPosition();//現在位置、縮小マップ上の位置を指定するためにこれを用いて移動させないといけない
		addShape->Move(minimapPosition+pos*minimapRate-pos);
		addShape->Resize(addShape->GetRetResize()*minimapRate);
		//条件付き追加
		if(addShape->JudgeCross(conditionShape)){
			//なんか条件を書く、実装はまだ（developから派生させて書きたいため）
			shapeList.push_back(addShape);
		}
	};
	const auto addMinimapObject0=[&addFunc,this](std::shared_ptr<Shape> &shape){
		//ユニット同士を結ぶ線分の端点
		const Vector2D p0=minimapPos[0]+m_goodLogInfo->pAttackedUnit->getPos()*minimapRate
			,p1=minimapPos[0]+m_goodLogInfo->pOperateUnit->getPos()*minimapRate;
		//p0p1に垂直なベクトルで、p0p1の中点からhだけ進んだ所にある点をp2とすると|p0p2|+|p1p2|がpAttackedUnitの移動距離になるようなベクトル
		Vector2D h=(p1-p0).turn(M_PI_4);
		const float unitMoveDistance=m_goodLogInfo->pAttackedUnit->GetMaxMoveDistance();
		h=h.norm()*std::powf(unitMoveDistance*unitMoveDistance-h.sqSize(),0.5f)*0.5f;
		//図形を作成して条件次第でshapeをリストに追加
		addFunc(shape,minimapPos[0],&MyPolygon(p0-h,{p0+h,p1+h,p1-h},Shape::Fix::e_dynamic));
	};
	const auto addMinimapObject1=[&addFunc,this](std::shared_ptr<Shape> &shape){
		//ユニット同士を結ぶ線分の端点
		const Vector2D p0=minimapPos[1]+m_badLogInfo->pAttackedUnit->getPos()*minimapRate
			,p1=minimapPos[1]+m_badLogInfo->pOperateUnit->getPos()*minimapRate;
		//p0p1に垂直なベクトルで、p0p1の中点からhだけ進んだ所にある点をp2とすると|p0p2|+|p1p2|がpAttackedUnitの移動距離になるようなベクトル
		Vector2D h=(p1-p0).turn(M_PI_4);
		const float unitMoveDistance=m_badLogInfo->pAttackedUnit->GetMaxMoveDistance();
		h=h.norm()*std::powf(unitMoveDistance*unitMoveDistance-h.sqSize(),0.5f)*0.5f;
		//図形を作成して条件次第でshapeをリストに追加
		addFunc(shape,minimapPos[1],&MyPolygon(p0-h,{p0+h,p1+h,p1-h},Shape::Fix::e_dynamic));
//		addFunc(shape,minimapPos[1],&Edge(p0,p1-p0,Shape::Fix::e_dynamic));
	};
	//ユニットデータ以外の障害物の格納
	for(const BattleObject *object:m_battleSceneData->m_field){
		if(object->GetType()!=BattleObject::Type::e_unit){
			//当たり判定図形を引き出して追加
			addMinimapObject0(object->GetHitJudgeShape()->VCopy());
			addMinimapObject1(object->GetHitJudgeShape()->VCopy());
		}
	}
	//ユニットデータの格納
	for(size_t i=0,siz=m_goodLogInfo->GetUnitList().size();i<siz;i++){
		//当たり判定図形を引き出して追加
		if(&m_goodLogInfo->GetUnitList()[i]!=m_goodLogInfo->pOperateUnit && &m_goodLogInfo->GetUnitList()[i]!=m_goodLogInfo->pAttackedUnit){
			addMinimapObject0(m_goodLogInfo->GetUnitList()[i].GetHitJudgeShape()->VCopy());
		}
	}
	for(size_t i=0,siz=m_badLogInfo->GetUnitList().size();i<siz;i++){
		//当たり判定図形を引き出して追加
		if(&m_badLogInfo->GetUnitList()[i]!=m_badLogInfo->pOperateUnit && &m_badLogInfo->GetUnitList()[i]!=m_badLogInfo->pAttackedUnit){
			addMinimapObject1(m_badLogInfo->GetUnitList()[i].GetHitJudgeShape()->VCopy());
		}
	}
	//ワークの作成
	m_reflectionWork=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::ObjectClick(shapeList));
}
