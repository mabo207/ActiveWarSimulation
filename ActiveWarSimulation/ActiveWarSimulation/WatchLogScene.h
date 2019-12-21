#ifndef DEF_WATCHLOGSCENE_H
#define DEF_WATCHLOGSCENE_H

#include"GameScene.h"
#include"BattleSceneData.h"

//���O���{��������
class WatchLogScene:public GameScene{
public:
	class WatchLogSceneFactory:public SceneFactory{
	public:
		WatchLogSceneFactory(const std::string &i_logFileName):logFileName(i_logFileName){}
		~WatchLogSceneFactory()=default;
		std::shared_ptr<GameScene> CreateIncompleteScene()const{
			return std::shared_ptr<GameScene>(new WatchLogScene(logFileName));
		}

	private:
		const std::string logFileName;
	};

	WatchLogScene(const std::string &logFileName);
	~WatchLogScene();
	void InitCompletely();//���C���X���b�h�ȊO�ōs�����Ƃ��ł��鏉���������Q
	void Activate();//���������I�����A����Scene�̓��삪�n�܂鎞�ɍs�������BBGM�̍Đ������ȂǁB
	int Calculate();
	void Draw()const;

private:
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;//��ʑJ�ڂ����鎞�ɁA�ǂ��Ɉړ����邩�����߂�֐��B���̒l�̐ݒ���@�́A�N���X���ƂɎ������@��ς��ėǂ��B
	void AdaptLog();

	size_t m_logIndex;
	std::shared_ptr<BattleSceneData> m_battleSceneData;
	std::vector<std::shared_ptr<LogElement>> m_logList;
};

#endif // !DEF_WATCHLOGSCENE_H
