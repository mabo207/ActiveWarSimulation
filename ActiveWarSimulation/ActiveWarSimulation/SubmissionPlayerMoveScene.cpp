#include"SubmissionPlayerMoveScene.h"
#include"DxLib.h"
#include"CommonConstParameter.h"

namespace{
	int GetSubmissionX(){
		return CommonConstParameter::gameResolutionX-SelfDecideSubmission::s_submissionWidth;
	}
}

//------------SubmissionPlayerMoveScene----------------
SubmissionPlayerMoveScene::SubmissionPlayerMoveScene(const std::shared_ptr<BattleSceneData> &battleSceneData)
	:TutorialPlayerMoveScene(battleSceneData)
	,m_submissionPosition(GetSubmissionX(),-SelfDecideSubmission::s_submissionHeight,10,Easing::TYPE_IN,Easing::FUNCTION_LINER,0.0)
	,m_notOperateFrame(0)
{}

SubmissionPlayerMoveScene::~SubmissionPlayerMoveScene(){}

int SubmissionPlayerMoveScene::thisCalculate(){
	const Vector2D beforeMousePos=m_mousePosJustBefore;
	const int retPal=TutorialPlayerMoveScene::thisCalculate();
	//無操作時間の更新(マウスのみで)(この時点でm_mousePosJustBeforeは現在のマウス位置になっていることに注意)
	if(beforeMousePos!=m_mousePosJustBefore){
		m_notOperateFrame=0;
	} else{
		m_notOperateFrame++;
	}
	printfDx("frame:%d y:%d targety:%d\n",m_notOperateFrame,m_submissionPosition.GetY(),m_submissionPosition.GetendY());
	//説明グラフィックが出ていない時、サブミッションの位置の更新
	if(m_tutorialBattleSceneData->m_tutorialData.empty() || m_tutorialBattleSceneData->m_tutorialData[0]->m_kind!=TutorialBattleSceneData::TutorialBase::TutorialKind::e_explain){
		if(m_notOperateFrame==0){
			//操作されたら、画面外に移動させる
			//動かし続けても外に移動しなくなってしまうため、bool値は工夫する
			m_submissionPosition.SetTarget(GetSubmissionX(),-SelfDecideSubmission::s_submissionHeight,m_submissionPosition.GetendY()!=-SelfDecideSubmission::s_submissionHeight);
		} else if(m_notOperateFrame==120){
			//2秒無操作状態が続いたら画面内に移動させる
			m_submissionPosition.SetTarget(GetSubmissionX(),0,true);
		} else{
			m_submissionPosition.Update();
		}
	}

	return retPal;
}

void SubmissionPlayerMoveScene::thisDraw()const{
	TutorialPlayerMoveScene::thisDraw();
	//サブミッション描画
	if(m_battleSceneData->m_submissionRunFlag){
		m_battleSceneData->m_scoreObserver->GetSubmission().DrawSubmission(m_submissionPosition.GetX(),m_submissionPosition.GetY());
	}
}
