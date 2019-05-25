#ifndef DEF_BATTLESCENE_H
#define DEF_BATTLESCENE_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"

//�o�g���̃Q�[���v���C��ʑS�ʂ̊Ǘ����s���N���X
class BattleScene:public MainControledGameScene{
	//�^�E�񋓑�
public:
	class BattleSceneFactory:public MainSceneFactory{
	public:
		BattleSceneFactory(const std::string &stageDirName,const std::string &title,const int level);
		~BattleSceneFactory();
		std::shared_ptr<MainControledGameScene> CreateScene()const;
	private:
		const std::string m_stageDirName;
		const std::string m_title;
		const int m_level;
	};

	//�萔
protected:
	static const int resetInterval;

public:

	//�ϐ�
protected:
	//�X�e�[�W��蒼�������֘A
	int m_resetFrame;//���Z�b�g�����ɂ������Ă��鎞��
	bool m_resetFlag;//���Z�b�g���������ǂ���

	//�Q�[���̐i�s���Ǘ�����ϐ�
	std::shared_ptr<BattleSceneElement> m_sceneData;//��Ԋ��ƂȂ�V�[��

	//�Q�[���Ɏg�p����ϐ�
	std::shared_ptr<BattleSceneData> m_battleSceneData;//�Q�[�����S�̂Ŏg���f�[�^
	
	//�֐�
protected:
	BattleScene(std::shared_ptr<BattleSceneData> battleSceneData);//�p���N���X�p�R���X�g���N�^
	BattleScene(const std::string &stageDirName,const std::string &titleName,const int stageLevel);
	virtual std::shared_ptr<BattleSceneElement> VGetSwitchUnitScene()const;//SwitchUnitScene��DemoSwitchUnitScene�̂ǂ������g����
	void ResetGame();

public:
	virtual ~BattleScene();
	virtual int Calculate();
	virtual void Draw()const;
	std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const;
};


#endif // !DEF_BATTLESCENE_H

#pragma once
