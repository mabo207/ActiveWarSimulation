#ifndef DEF_FADEOUTSCENE_H
#define DEF_FADEOUTSCENE_H

//�V�[���ƃV�[���̍��Ԃ̃t�F�[�h�A�E�g�����錄�ԏ��
//���̃V�[����VGetNextScene()���Ă΂ꂽ���ɍ쐬�����
#include"GameScene.h"
#include"ToolsLib.h"

// �t�F�[�h�A�E�g����V�[��
class FadeOutScene: public GameScene
{
public:
	class FadeOutSceneFactory:public SceneFactory{
	public:
		FadeOutSceneFactory(const std::shared_ptr<GameScene> &i_previousScene,const std::shared_ptr<GameScene::SceneFactory> &i_nextFactory,const int i_maxFrame)
			:previousScene(i_previousScene),nextFactory(i_nextFactory),maxFrame(i_maxFrame){}
		~FadeOutSceneFactory(){}
		std::shared_ptr<GameScene> CreateIncompleteScene()const{
			return std::shared_ptr<GameScene>(new FadeOutScene(previousScene,nextFactory,maxFrame));
		}

	private:
		const std::shared_ptr<GameScene> previousScene;
		const std::shared_ptr<GameScene::SceneFactory> nextFactory;
		const int maxFrame;
	};

	virtual ~FadeOutScene();
	void InitCompletely();
	void Activate();
	int Calculate();
	void Draw()const;

protected:
	FadeOutScene(const std::shared_ptr<GameScene> &previousScene,const std::shared_ptr<GameScene::SceneFactory> &nextFactory,int maxFrame);
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;

private:
	Easing m_drawAlpha;		// �`�擧���x��x�ɊǗ�
	const std::shared_ptr<GameScene> m_previousScene;	// ���O��ʁB�`��̂��߂Ɏg���B
	const std::shared_ptr<GameScene::SceneFactory> m_nextFactory;	// ���̏�ʁA�`��̏��͓������̂Ŏ��̂��쐬���Ă���
	int m_afterAlphaEndFrame;	// m_drawAlpha�̕ϓ����I�����Ă��牽�t���[���o�������B2f�o������łȂ��Ɖ�ʂ����S�ɈÓ]���Ȃ��B
};

#endif // !DEF_FADEINSCENE_H