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

#include"LineDraw.h"

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
SubmissionReflectionScene::MinimapDrawInfo::MinimapDrawInfo(const std::shared_ptr<const LogElement> &log)
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
			Unit u=*logData.punit;
			u.Warp(logData.pos);
			u.AddHP(logData.hp-u.GetBattleStatus().HP);
			u.SetOP(logData.op);
			u.SetPenetratable(Unit::Team::e_player);
			//格納
			unitList.push_back(u);
			//operateIndex,attackedIndexの探索、関数が存在するかどうか気をつける
			if(getOperateUnit && logData.punit==getOperateUnit()){
				operateIndex=index;
			}
			if(getAttackedUnit && logData.punit==getAttackedUnit()){
				attackedIndex=index;
			}
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
	m_goodLogInfo.emplace(reflectionInfo.m_goodLog.second);
	m_badLogInfo.emplace(reflectionInfo.m_badLog.second);
	//m_reflectionWorkの初期化（暫定）
	const Vector2D goodLogStart=minimapPos[0]+m_goodLogInfo->pOperateUnit->getPos()*minimapRate;
	const Vector2D goodLogEnd=minimapPos[0]+m_goodLogInfo->pAttackedUnit->getPos()*minimapRate;
	const Vector2D badLogStart=minimapPos[1]+m_badLogInfo->pOperateUnit->getPos()*minimapRate;
	const Vector2D badLogEnd=minimapPos[1]+m_badLogInfo->pAttackedUnit->getPos()*minimapRate;
	m_reflectionWork=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::LineDraw(
		{Edge(goodLogStart,goodLogEnd-goodLogStart,Shape::Fix::e_ignore)
		,Edge(badLogStart,badLogEnd-badLogStart,Shape::Fix::e_ignore)}
	));
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
