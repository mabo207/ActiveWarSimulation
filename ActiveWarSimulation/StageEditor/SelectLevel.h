#ifndef DEF_SELECTLEVEL_H
#define DEF_SELECTLEVEL_H

#include"ButtonHaving.h"
#include"StageLevel.h"

//�ǂݍ��݁E�o�͂���X�e�[�W���x����I������
class SelectLevel:public ButtonHaving{
public:
	struct SelectLevelButton:public Button{
	public:
		SelectLevelButton(Vector2D buttonPos,Vector2D buttonVec,StageLevel level);
		~SelectLevelButton()=default;
		void ButtonDraw(int font,int fillFlag)const;//�{�^�����̂�`�悷��
		bool JudgeInButton(Vector2D point)const;//�{�^���̒��ɂ��邩�̔���
		bool JudgeButtonPushed()const;//�{�^�����������삪���ꂽ���𔻒肷��
		void PushedProcess(EditActionSettings &settings)const;//�{�^���������ꂽ���ɍs������

	private:
		Vector2D m_buttonPos;
		Vector2D m_buttonVec;
		StageLevel m_level;
	};

	SelectLevel(Vector2D lightUpPos,Vector2D lightUpVec,StageLevel level);
	~SelectLevel()=default;
	void LightUpButton()const;
	std::string GetUnitListFileName()const;

private:
	Vector2D m_lightUpPos;
	Vector2D m_lightUpVec;
	StageLevel m_level;
};

#endif // !DEF_SELECTLEVEL_H