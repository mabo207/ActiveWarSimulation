#ifndef DEF_FADEINSCENE_H
#define DEF_FADEINSCENE_H

//�V�[���ƃV�[���̍��Ԃ̃t�F�[�h�C�������錄�ԏ��
//���̃V�[����FadeInScene�̍쐬�Ɠ����ɍ쐬�����
#include"GameScene.h"
#include"ToolsLib.h"

// �t�F�[�h�C������V�[��
class FadeInScene: public GameScene
{
public:
	class FadeInSceneFactory:public SceneFactory{
	public:
		FadeInSceneFactory(const std::shared_ptr<GameScene::SceneFactory> &i_nextFactory,const int i_maxFrame)
			:nextFactory(i_nextFactory),maxFrame(i_maxFrame){}
		~FadeInSceneFactory(){}
		std::shared_ptr<GameScene> CreateScene()const{
			return std::shared_ptr<GameScene>(new FadeInScene(nextFactory,maxFrame));
		}

	private:
		const std::shared_ptr<GameScene::SceneFactory> nextFactory;
		const int maxFrame;
	};

	virtual ~FadeInScene();
	int Calculate();
	void Draw()const;

protected:
	FadeInScene(const std::shared_ptr<GameScene::SceneFactory> &nextFactory,int maxFrame);
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;

private:
	Easing m_drawAlpha;		// �`�擧���x��x�ɊǗ�
	const std::shared_ptr<GameScene> m_nextScene;	// ���̏�ʁA�`��̏��͓������̂Ŏ��̂��쐬���Ă���
	int m_afterAlphaEndFrame;	// m_drawAlpha�̕ϓ����I�����Ă��牽�t���[���o�������B2f�o������łȂ��Ɖ�ʂ����S�ɈÓ]���Ȃ��B
};

#endif // !DEF_FADEINSCENE_H