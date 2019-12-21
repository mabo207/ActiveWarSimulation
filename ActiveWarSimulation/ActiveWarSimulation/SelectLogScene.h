#ifndef DEF_SELECTLOGSCENE_H
#define DEF_SELECTLOGSCENE_H

#include"GameScene.h"
#include<string>
#include<vector>

class SelectLogScene:public GameScene{
public:
	class SelectLogSceneFactory:public SceneFactory{
	public:
		SelectLogSceneFactory(){}
		~SelectLogSceneFactory(){}
		std::shared_ptr<GameScene> CreateIncompleteScene()const{
			return std::shared_ptr<GameScene>(new SelectLogScene());
		}
	};
	
	SelectLogScene();
	~SelectLogScene();
	void InitCompletely();//���C���X���b�h�ȊO�ōs�����Ƃ��ł��鏉���������Q
	void Activate();//���������I�����A����Scene�̓��삪�n�܂鎞�ɍs�������BBGM�̍Đ������ȂǁB
	int Calculate();
	void Draw()const;

private:
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;//��ʑJ�ڂ����鎞�ɁA�ǂ��Ɉړ����邩�����߂�֐��B

	std::vector<std::string> m_fileNameList;
	std::vector<std::string>::const_iterator m_selectFileIt;
};

#endif // !DEF_SELECTLOGSCENE_H
