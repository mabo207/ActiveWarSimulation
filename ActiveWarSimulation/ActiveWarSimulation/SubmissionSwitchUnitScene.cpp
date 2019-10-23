#include"SubmissionSwitchUnitScene.h"
#include"SelectSubmissionScene.h"
#include"SubmissionPlayerMoveScene.h"
#include"SubmissionReflectionScene.h"

//-------------SubmissionSwitchUnitScene--------------
SubmissionSwitchUnitScene::SubmissionSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:SwitchUnitInitOrderScene(battleSceneData)
{}

SubmissionSwitchUnitScene::~SubmissionSwitchUnitScene(){}

int SubmissionSwitchUnitScene::UpdateNextScene(int index){
	const int retPal=SwitchUnitScene::UpdateNextScene(index);
	if(retPal==SceneKind::e_clear){
		//�X�e�[�W�N���A���́A���݂�m_nextScene���T�u�~�b�V�����U��Ԃ��ʂɐ؂�ւ���
		const std::shared_ptr<BattleSceneElement> clearScene=m_nextScene;
		m_nextScene.reset(new SubmissionReflectionScene(m_battleSceneData,clearScene));
		return SceneKind::e_submissionReflection;//�Ԃ�l���ύX
	} else{
		return retPal;
	}
}

std::shared_ptr<BattleSceneElement> SubmissionSwitchUnitScene::GetPlayerMoveScene()const{
	return std::shared_ptr<BattleSceneElement>(new SubmissionPlayerMoveScene(m_battleSceneData));
}
