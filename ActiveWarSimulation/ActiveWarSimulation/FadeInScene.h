#ifndef DEF_FADEINSCENE_H
#define DEF_FADEINSCENE_H

//�V�[���ƃV�[���̍��Ԃ̃t�F�[�h�C�������錄�ԏ��
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
	std::shared_ptr<GameScene> VGetNextScene()const;

protected:
	FadeInScene(const std::shared_ptr<GameScene::SceneFactory> &nextFactory,int maxFrame);

private:
	Easing m_drawAlpha;		// �`�擧���x��x�ɊǗ�
	const std::shared_ptr<GameScene> m_nextScene;	// ���̏�ʁA�`��̏��͓������̂Ŏ��̂��쐬���Ă���
};



#endif // !DEF_FADEINSCENE_H
