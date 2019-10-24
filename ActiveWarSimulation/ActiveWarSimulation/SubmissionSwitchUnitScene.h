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
	int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	std::shared_ptr<BattleSceneElement> GetPlayerMoveScene()const;
};

#endif // !DEF_SUBMISSIONSWITCHUNITSCENE_H
