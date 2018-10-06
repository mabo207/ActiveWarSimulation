#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"

//�o�g���̃Q�[���v���C��ʑS�ʂ̊Ǘ����s���N���X
class BattleScene:public MainControledGameScene{
	//�^�E�񋓑�
public:
	struct RequiredInfoToMakeBattleScene:public RequiredInfoToMakeClass{
		std::string m_stagename;
		RequiredInfoToMakeBattleScene(const std::string &stagename):m_stagename(stagename){}
		Kind GetKind()const{
			return e_battleScene;
		}
	};

	//�萔
protected:
	static const int resetInterval;

public:

	//�ϐ�
protected:
	//�X�e�[�W��蒼�������֘A
	int m_resetFlame;//���Z�b�g�����ɂ������Ă��鎞��
	bool m_resetFlag;//���Z�b�g���������ǂ���

	//�Q�[���̐i�s���Ǘ�����ϐ�
	std::shared_ptr<BattleSceneElement> m_sceneData;//��Ԋ��ƂȂ�V�[��

	//�Q�[���Ɏg�p����ϐ�
	std::shared_ptr<BattleSceneData> m_battleSceneData;//�Q�[�����S�̂Ŏg���f�[�^
	
	//�֐�
protected:
	virtual std::shared_ptr<BattleSceneElement> VGetSwitchUnitScene()const;//SwitchUnitScene��DemoSwitchUnitScene�̂ǂ������g����

public:
	BattleScene(const char *stagename);
	virtual ~BattleScene();
	int Calculate();
	void Draw()const;
	std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
