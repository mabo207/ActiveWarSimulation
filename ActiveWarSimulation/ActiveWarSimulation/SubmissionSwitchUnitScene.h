#ifndef DEF_SUBMISSIONSWITCHUNITSCENE_H
#define DEF_SUBMISSIONSWITCHUNITSCENE_H

//�T�u�~�b�V������ݒ肷��K�v������ꍇ�̃g�b�v�ɐݒu����SwitchUnitScene
//�C���X�^���X�쐬���ɁAm_nextScene��SelectSubmissionScene�𐶐����邾���ŁA���̋@�\�͕ς��Ȃ��B

#include"SwitchUnitScene.h"

class SubmissionSwitchUnitScene:public SwitchUnitScene{
public:
	SubmissionSwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~SubmissionSwitchUnitScene();
};

#endif // !DEF_SUBMISSIONSWITCHUNITSCENE_H
