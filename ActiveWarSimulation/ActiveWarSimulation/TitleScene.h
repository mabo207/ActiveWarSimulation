#ifndef DEF_TITLESCENE_H
#define DEF_TITLESCENE_H

#include"GameScene.h"
#include"ToolsLib.h"
#include"Shape.h"
#include<memory>
#include<array>
#include<string>

//�^�C�g����ʂ̃N���X
class TitleScene:public GameScene{
	//�^�E�񋓑�
private:
	struct SelectItem{
		//�^�C�g����ʂőI���ł��鍀��
		enum Kind{
			e_stageSelect
			,e_tutorial
			,e_tutorial_2
			,e_demo
			,e_gameFinish
			,COUNTER
		};
		static std::string GetString(const Kind kind);
	};
public:
	//GameScene���p�����Ă��邽�߃N���X��factory�N���X�����K�v������
	class TitleSceneFactory:public SceneFactory{
	public:
		//�N���X�����̂ɕK�v�ȃf�[�^�͂Ȃ�
		TitleSceneFactory();
		virtual ~TitleSceneFactory();
		std::shared_ptr<GameScene> CreateScene()const;
	};
	
	//�萔
private:
	static const Vector2D strPos[SelectItem::COUNTER];

	//�ϐ�
protected:
	int m_frame;//�����ɃA�j���[�V���������邽��
	Vector2D m_mousePosJustBefore;//���O�t���[���ɂ�����}�E�X�̈ʒu
	SelectItem::Kind m_selectItem;//���ݑI�����Ă��鍀��
	std::array<std::shared_ptr<Shape>,SelectItem::COUNTER> m_hitJudgeShapeVec;

	//�O���t�B�b�N
	const int m_backPic;//�w�i
	const int m_itemFont;//�I�����ڂ̃t�H���g

	//bgm
	const int m_bgm;

	//���ʉ�
	const int m_aimchangeSound;

	//�֐�
private:
	std::shared_ptr<Shape> MakeHexagon(const Vector2D center,const float size)const;
	int thisCalculate();//SelectItem::COUNTER:����ێ� 0~SelectItem::COUNTER-1:�J��

protected:
	TitleScene();

public:
	virtual ~TitleScene();
	int Calculate();
	void Draw()const;
	std::shared_ptr<GameScene> VGetNextScene()const;
};

#endif // !DEF_TITLESCENE_H
