#ifndef DEF_CREDITSCENE_H
#define DEF_CREDITSCENE_H

#include"GameScene.h"
#include<string>
#include<vector>
#include"BGM.h"

class CreditScene:public GameScene{
public:
	class CreditSceneFactory:public SceneFactory{
	public:
		CreditSceneFactory()=default;
		~CreditSceneFactory()=default;
		std::shared_ptr<GameScene> CreateIncompleteScene()const;
	};

	~CreditScene();
	void InitCompletely();//���C���X���b�h�ȊO�ōs�����Ƃ��ł��鏉���������Q
	void Activate();//���������I�����A����Scene�̓��삪�n�܂鎞�ɍs�������BBGM�̍Đ������ȂǁB
	int Calculate();
	void Draw()const;

private:
	struct CreditPair{
		//��E�Ƃ��̖�E�̐l�̈ꗗ���y�A�ɂ�������
		const std::string profession;
		const std::vector<std::string> nameVec;
		CreditPair(const std::string &i_profession,const std::vector<std::string> &i_nameVec)
			:profession(i_profession),nameVec(i_nameVec){}
		~CreditPair()=default;
	};

	CreditScene();
	std::shared_ptr<GameScene> VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const;//��ʑJ�ڂ����鎞�ɁA�ǂ��Ɉړ����邩�����߂�֐��B���̒l�̐ݒ���@�́A�N���X���ƂɎ������@��ς��ėǂ��B

	//�ϐ�
	std::vector<CreditPair> m_creditList;
	int m_strY;//������`��ʒu(y)
	int m_strHeight;
	//�t�H���g
	const int m_professionFont;
	const int m_nameFont;
	//�w�i
	const int m_backPic;
	//BGM
	const Resource::BGM m_bgm;
};

#endif // !DEF_CREDITSCENE_H
