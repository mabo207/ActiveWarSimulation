#ifndef DEF_SUBMISSIONSWITCHUNITSCENE_H
#define DEF_SUBMISSIONSWITCHUNITSCENE_H

//�T�u�~�b�V������ݒ肷��K�v������ꍇ�̃g�b�v�ɐݒu����SwitchUnitScene
//�C���X�^���X�쐬���ɁAm_nextScene��SelectSubmissionScene�𐶐����邾���ŁA���̋@�\�͕ς��Ȃ��B

#include"SwitchUnitInitOrderScene.h"

class SubmissionSwitchUnitScene:public SwitchUnitInitOrderScene{
public:
	SubmissionSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~SubmissionSwitchUnitScene();

protected:
	std::shared_ptr<BattleSceneElement> GetPlayerMoveScene()const;
};

#endif // !DEF_SUBMISSIONSWITCHUNITSCENE_H
