#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include<vector>
#include"GameScene.h"
#include"Unit.h"
#include"Terrain.h"

//�o�g����ʕ\���N���X
class BattleScene:public GameScene{
	//�^�E�񋓑�
public:

	//�萔
protected:

public:

	//�ϐ�
protected:
	//�Q�[���̐i�s���Ǘ�����ϐ�


	//�Q�[���Ɏg�p����ϐ�
	std::vector<BattleObject *> m_field;//�t�B�[���h�ɑ��݂���I�u�W�F�N�g�ꗗ(���I�|�C���^�̔z��)
	Unit *m_operateUnit;//���ݑ���ł��郆�j�b�g(�ÓI�|�C���^)

	//�O���t�B�b�N�֘A�̕ϐ�
	std::shared_ptr<Terrain> m_Window;//�E�C���h�E�S�̂�\������(�Ίp��)


	//�֐�
protected:
	void UpdateFix();
	bool PositionUpdate();//���j�b�g�̈ʒu���X�V�Am_operateUnit�Ɉړ����삪������true��Ԃ��B

public:
	BattleScene(const char *stagename);
	~BattleScene();
	int Calculate();
	void Draw()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
