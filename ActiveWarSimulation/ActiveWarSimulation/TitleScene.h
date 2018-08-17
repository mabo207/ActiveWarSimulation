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
			,e_gameFinish
			,COUNTER
		};
		static std::string GetString(const Kind kind);
	};

	//�萔
	static const Vector2D strPos[SelectItem::COUNTER];

	//�ϐ�
protected:
	int m_flame;//�����ɃA�j���[�V���������邽��
	Vector2D m_mousePosJustBefore;//���O�t���[���ɂ�����}�E�X�̈ʒu
	SelectItem::Kind m_selectItem;//���ݑI�����Ă��鍀��
	std::array<std::shared_ptr<Shape>,SelectItem::COUNTER> m_hitJudgeShapeVec;
	

	//�O���t�B�b�N
	const int m_backPic;//�w�i
	const int m_titleLogo;//�^�C�g�����S
	const int m_itemFont;//�I�����ڂ̃t�H���g

	//�֐�
private:
	std::shared_ptr<Shape> MakeHexagon(const Vector2D center,const float size)const;

public:
	TitleScene();
	~TitleScene();
	int Calculate();
	void Draw()const;

};

#endif // !DEF_TITLESCENE_H
