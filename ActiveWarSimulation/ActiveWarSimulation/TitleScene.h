#ifndef DEF_TITLESCENE_H
#define DEF_TITLESCENE_H

#include"GameScene.h"
#include"ToolsLib.h"
#include"Shape.h"
#include<memory>
#include<array>
#include<string>

//�^�C�g����ʂ̃N���X
class TitleScene:public MainControledGameScene{
	//�^�E�񋓑�
private:
	struct SelectItem{
		//�^�C�g����ʂőI���ł��鍀��
		enum Kind{
			e_stageSelect
			,e_demo
			,e_gameFinish
			,COUNTER
		};
		static std::string GetString(const Kind kind);
	};
public:
	struct RequiredInfoToMakeTitleScene:public RequiredInfoToMakeClass{
		//�N���X�����̂ɕK�v�ȃf�[�^�͂Ȃ�
		RequiredInfoToMakeTitleScene(){}
		Kind GetKind()const{
			return e_titleScene;
		}
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
	std::shared_ptr<GameScene> m_nextScene;//���̃V�[���B���ꂪnullptr�Ȃ�A�^�C�g���̏������s���B

	//���̃N���X�����̂ɕK�v�ȃf�[�^
	std::shared_ptr<RequiredInfoToMakeClass> m_reqInfo;//�����ւ̃|�C���^��n�����ƂŁA���̃N���X�ł����̒l��M���B�����p���āA���̃N���X����߂鎞�ɁA���ǂ̃N���X�ɍs���΂������Ȃǂ��v�Z�ł���B

	//�O���t�B�b�N
	const int m_backPic;//�w�i
	const int m_titleLogo;//�^�C�g�����S
	const int m_itemFont;//�I�����ڂ̃t�H���g

	//bgm
	const int m_bgm;

	//���ʉ�
	const int m_aimchangeSound;

	//�֐�
private:
	std::shared_ptr<Shape> MakeHexagon(const Vector2D center,const float size)const;
	int thisCalculate();//SelectItem::COUNTER:����ێ� 0~SelectItem::COUNTER-1:�J��
	void thisDraw()const;

public:
	TitleScene();
	~TitleScene();
	int Calculate();
	void Draw()const;
	std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const;
};

#endif // !DEF_TITLESCENE_H
