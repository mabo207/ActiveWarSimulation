#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"

//�o�g���̃Q�[���v���C��ʑS�ʂ̊Ǘ����s���N���X
class BattleScene:public GameScene{
	//�^�E�񋓑�
public:
	
	//�萔
protected:
	
public:

	//�ϐ�
protected:
	//�Q�[���̐i�s���Ǘ�����ϐ�
	std::shared_ptr<BattleSceneElement> m_sceneData;//��Ԋ��ƂȂ�V�[��

	//�Q�[���Ɏg�p����ϐ�
	std::shared_ptr<BattleSceneData> m_battleSceneData;//�Q�[�����S�̂Ŏg���f�[�^
	
	//�֐�
protected:

public:
	BattleScene(const char *stagename);
	~BattleScene();
	int Calculate();
	void Draw()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
