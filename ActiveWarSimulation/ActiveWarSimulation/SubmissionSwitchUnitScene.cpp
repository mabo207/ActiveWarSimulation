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
		//ステージクリア時は、現在のm_nextSceneをサブミッション振り返り場面に切り替える
		const std::shared_ptr<BattleSceneElement> clearScene=m_nextScene;
		m_nextScene.reset(new SubmissionReflectionScene(m_battleSceneData,clearScene));
		return SceneKind::e_submissionReflection;//返り値も変更
	} else{
		return retPal;
	}
}

std::shared_ptr<BattleSceneElement> SubmissionSwitchUnitScene::GetPlayerMoveScene()const{
	return std::shared_ptr<BattleSceneElement>(new SubmissionPlayerMoveScene(m_battleSceneData));
}
