#ifndef DEF_SETTINGFUNCTION_H
#define DEF_SETTINGFUNCTION_H

#include"ButtonHaving.h"
#include<functional>

class EditActionSettings;

//�{�^�������������ɉ������Ă����֐�
class SettingFunction:public ButtonHaving{
public:
	struct SettingFunctionButton:public Button{
	public:
		SettingFunctionButton(Vector2D buttonPos,Vector2D buttonVec,const std::string &buttonName,const std::function<void(EditActionSettings &)> &pushedCallback);
		~SettingFunctionButton()=default;
		void ButtonDraw(int font,int fillFlag)const;//�{�^�����̂�`�悷��
		bool JudgeInButton(Vector2D point)const;//�{�^���̒��ɂ��邩�̔���
		bool JudgeButtonPushed()const;//�{�^�����������삪���ꂽ���𔻒肷��
		void PushedProcess(EditActionSettings &settings)const;//�{�^���������ꂽ���ɍs������

	private:
		Vector2D m_buttonPos;
		Vector2D m_buttonVec;
		const std::string m_buttonName;
		const std::function<void(EditActionSettings &)> m_pushedCallback;
	};

	SettingFunction(Vector2D lightUpPos,Vector2D lightUpVec,const std::function<void(EditActionSettings &)> &pushedCallback,EditActionSettings &settings);
	~SettingFunction()=default;
	void LightUpButton()const;

private:
	Vector2D m_lightUpPos;
	Vector2D m_lightUpVec;
};

#endif // !DEF_SETTINGFUNCTION_H
