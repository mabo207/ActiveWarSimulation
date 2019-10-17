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
	//�����쎞�Ԃ̍X�V(�}�E�X�݂̂�)(���̎��_��m_mousePosJustBefore�͌��݂̃}�E�X�ʒu�ɂȂ��Ă��邱�Ƃɒ���)
	if(beforeMousePos!=m_mousePosJustBefore){
		m_notOperateFrame=0;
	} else{
		m_notOperateFrame++;
	}
	printfDx("frame:%d y:%d targety:%d\n",m_notOperateFrame,m_submissionPosition.GetY(),m_submissionPosition.GetendY());
	//�����O���t�B�b�N���o�Ă��Ȃ����A�T�u�~�b�V�����̈ʒu�̍X�V
	if(m_tutorialBattleSceneData->m_tutorialData.empty() || m_tutorialBattleSceneData->m_tutorialData[0]->m_kind!=TutorialBattleSceneData::TutorialBase::TutorialKind::e_explain){
		if(m_notOperateFrame==0){
			//���삳�ꂽ��A��ʊO�Ɉړ�������
			//�����������Ă��O�Ɉړ����Ȃ��Ȃ��Ă��܂����߁Abool�l�͍H�v����
			m_submissionPosition.SetTarget(GetSubmissionX(),-SelfDecideSubmission::s_submissionHeight,m_submissionPosition.GetendY()!=-SelfDecideSubmission::s_submissionHeight);
		} else if(m_notOperateFrame==120){
			//2�b�������Ԃ����������ʓ��Ɉړ�������
			m_submissionPosition.SetTarget(GetSubmissionX(),0,true);
		} else{
			m_submissionPosition.Update();
		}
	}

	return retPal;
}

void SubmissionPlayerMoveScene::thisDraw()const{
	TutorialPlayerMoveScene::thisDraw();
	//�T�u�~�b�V�����`��
	if(m_battleSceneData->m_submissionRunFlag){
		m_battleSceneData->m_scoreObserver->GetSubmission().DrawSubmission(m_submissionPosition.GetX(),m_submissionPosition.GetY());
	}
}
