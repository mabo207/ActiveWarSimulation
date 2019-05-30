#ifndef DEF_GAMESCENE_H
#define DEF_GAMESCENE_H

#include"ToolsLib.h"
#include<memory>

//��ʂ�\���N���X�����ׂ������̃C���^�[�t�F�[�X
//�����ꂾ������Α��N���X���瓮�������Ƃ��o����
class GameScene{
	//�^
public:
	class SceneFactory{
		//GameScene�N���X����邽�߂ɕK�v�ȃf�[�^�Q
	protected:
		SceneFactory(){}
		virtual ~SceneFactory(){}
	public:
		virtual std::shared_ptr<GameScene> CreateScene()const=0;//�������I�[�o�[���C�h�K�{�ɂ��āA�e�p����N���X��Factory����点��
	};
	//�萔

	//�ϐ�
	//�֐�
protected:
	GameScene(){}
	virtual ~GameScene(){}
	virtual std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const=0;//��ʑJ�ڂ����鎞�ɁA�ǂ��Ɉړ����邩�����߂�֐��B���̒l�̐ݒ���@�́A�N���X���ƂɎ������@��ς��ėǂ��B
	static std::shared_ptr<GameScene> CreateFadeOutInScene(const std::shared_ptr<GameScene> &thisSharedPtr,const std::shared_ptr<GameScene::SceneFactory> &nextFactory,const int fadeOutFrame,const int fadeInFrame);// �t�F�[�h�A�E�g���Ă���t�F�[�h�C������N���X���쐬����֗��֐�

public:
	virtual int Calculate()=0;
	virtual void Draw()const=0;
	//�ȉ��́AVGetNextScene()�̌Ăѕ������ɂ��邽�߂ɗp����
	static std::shared_ptr<GameScene> GetNextScene(const std::shared_ptr<GameScene> &scene){
		if(scene){
			//�������g�������ɓ����VNextScene���Ăяo��
			return scene->VGetNextScene(scene);
		}
		return std::shared_ptr<GameScene>();
	}
};

#endif // !DEF_GAMESCENE_H

