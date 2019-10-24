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
#include"AttackLog.h"

namespace {
	const int lineWidth=5;
	const unsigned int merginColor=GetColor(128,128,255);
	const float minimapRate=0.5f;
	const int minimapWidth=(int)(CommonConstParameter::mapSizeX*minimapRate);
	const int minimapHeight=(int)(CommonConstParameter::mapSizeY*minimapRate);
}

//---------------SubmissionReflectionScene-----------------
SubmissionReflectionScene::SubmissionReflectionScene(const std::shared_ptr<BattleSceneData> &battleSceneData,const std::shared_ptr<BattleSceneElement> &clearScene)
	:BattleSceneElement(SceneKind::e_submissionReflection)
	,m_battleSceneData(battleSceneData)
	,m_clearScene(clearScene)
	,m_operateCursor(LoadGraphEX(FilePath::graphicDir+"operatedCursor.png"))
{}

SubmissionReflectionScene::~SubmissionReflectionScene(){
	DeleteGraphEX(m_operateCursor);
}

void SubmissionReflectionScene::DrawResizedMap(int x,int y,const std::vector<Unit> &unitList,const std::shared_ptr<const LogElement> &log)const{
	//マージンの描画
	DrawBox(x-lineWidth,y-lineWidth,x+minimapWidth+lineWidth,y+minimapHeight+lineWidth,merginColor,TRUE);
	const Vector2D startPos((float)x,(float)y);
	//BattleSceneDataの描画関数は拡大縮小描画に対応していないので、独自に実装する
	//背景描画
	DrawExtendGraphExRateAssign(startPos.x,startPos.y,minimapRate,m_battleSceneData->m_mapPic,TRUE);
	//ユニットの描画
	for(size_t i=1,siz=unitList.size();i<siz+1;i++){
		const size_t index=i%siz;
		//ウインドウに入っていない物は描画しない
		//当たり判定図形のm_fix判定は行わない、そもそも死んでいるかどうかの判定なので、「実際のマップでは死んでいるが仮想マップでは生きているようにしたい」という要望には対応できないから
		//先頭ユニットは最後に描画する
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&unitList[index])){
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
			unitList[index].DrawUnit(unitList[index].getPos(),startPos,minimapRate,0,animationFlag,true,actionRangeDraw);
		}
	}
	//ユニットのHPゲージの描画
	for(size_t i=1,siz=unitList.size();i<siz+1;i++){
		const size_t index=i%siz;
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&unitList[index])){
			//ウインドウに入っていない物は描画しない
			unitList[index].DrawHPGage(unitList[index].getPos(),startPos,minimapRate);
		}
	}
	//操作ユニットのマーカーを表示
	if(!unitList.empty()){
		float dx,dy;
		GetGraphSizeF(m_operateCursor,&dx,&dy);
		const Vector2D pos=(unitList.begin()->getPos()+Vector2D(-dx/2.0f,-Unit::unitCircleSize-dy+10.0f))*minimapRate+startPos;
		DrawExtendGraphExRateAssign(pos.x,pos.y,minimapRate,m_operateCursor,TRUE);
	}
	//ダメージの表示
	if(log->GetLogKind()==LogElement::LogKind::e_attack){
		const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(log);
		if(attackLog){
			//攻撃時は、攻撃相手の上にダメージを表示
			const LogElement::UnitLogData attackedUnitData=attackLog->GetAimedUnitData();
			const Vector2D pos=attackedUnitData.pos*minimapRate+startPos;
			
		}
	}
}

int SubmissionReflectionScene::thisCalculate(){
	if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
		return SceneKind::e_clear;
	}

	return SceneKind::e_submissionReflection;
}

void SubmissionReflectionScene::thisDraw()const{
	//文章の描画
	DrawStringToHandle(30,30,"どちらの方が良いとされる行動でしょうか？",GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	//比較マップの描画
	const WholeReflectionInfo reflectionInfo=m_battleSceneData->m_scoreObserver->GetSubmission().GetReflectionInfo();
	if(reflectionInfo.m_badLog.second && reflectionInfo.m_goodLog.second){
		std::vector<Unit> goodUnitList,badUnitList;
		for(const LogElement::UnitLogData &logData:reflectionInfo.m_goodLog.second->m_unitDataList){
			//良いとされる盤面のユニットの作成
			Unit u=*logData.punit;
			u.Warp(logData.pos);
			u.AddHP(logData.hp-u.GetBattleStatus().HP);
			u.SetOP(logData.op);
			u.SetPenetratable(Unit::Team::e_player);
			//格納
			goodUnitList.push_back(u);
		}
		for(const LogElement::UnitLogData &logData:reflectionInfo.m_badLog.second->m_unitDataList){
			//悪いとされる盤面のユニットの作成
			Unit u=*logData.punit;
			u.Warp(logData.pos);
			u.AddHP(logData.hp-u.GetBattleStatus().HP);
			u.SetOP(logData.op);
			u.SetPenetratable(Unit::Team::e_player);
			//格納
			badUnitList.push_back(u);
		}
		DrawResizedMap(20,300,goodUnitList,reflectionInfo.m_goodLog.second);
		DrawResizedMap(980,300,badUnitList,reflectionInfo.m_badLog.second);
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
