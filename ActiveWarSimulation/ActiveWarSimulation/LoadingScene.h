#ifndef DEF_LOADINGSCENE_H
#define DEF_LOADINGSCENE_H

#include"GameScene.h"
#include<thread>
#include<atomic>

//���[�h���ɏ����̉�ʕύX�����������̌q���̏��
//�X���b�h��p���Ď�������
class LoadingScene:public GameScene{
public:
	class LoadingSceneFactory:public SceneFactory{
	public:
		LoadingSceneFactory(const std::shared_ptr<GameScene::SceneFactory> &nextFactory);
		~LoadingSceneFactory();
		std::shared_ptr<GameScene> CreateIncompleteScene()const{
			return std::shared_ptr<GameScene>(new LoadingScene(m_nextFactory));
		}
	private:
		const std::shared_ptr<GameScene::SceneFactory> m_nextFactory;
	};

	virtual ~LoadingScene();
	void InitCompletely();
	void Activate();
	int Calculate();
	void Draw()const;

protected:
	LoadingScene(const std::shared_ptr<GameScene::SceneFactory> &nextFactory);
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;
	
private:
	FpsMeasuring m_timer;	//�`��p�̃^�C�}�[
	std::thread m_loadingThread;	//���̏�ʂ�ǂݍ��ރX���b�h
	//�ȉ��͑S��m_loadingThread�ŕύX�����̂ŁA�X���b�h�쐬�ȍ~��mutex��p���ăA�N�Z�X����(atomic_bool�����̖������ʂ����Ă����)
	std::shared_ptr<GameScene> m_nextScene;	//�ǂݍ��ݐ�
	std::atomic_bool m_loadingEnd;	//�ǂݍ��݂��I��������ǂ����imutex�̏�Ԃ����ł́A�X���b�h�������n�߂�O��mutex�ł̏�Ԕ��肪�s��ꂽ��Ӑ}���Ȃ�����ɂȂ��Ă��܂����߁j
};

#endif // !DEF_LOADINGSCENE_H
