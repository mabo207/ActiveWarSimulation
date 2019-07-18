#ifndef DEF_SETTINGFUNCTION_H
#define DEF_SETTINGFUNCTION_H

#include"ButtonHaving.h"
#include<functional>

class EditActionSettings;

//ボタンを押した時に何かしてくれる関数
class SettingFunction:public ButtonHaving{
public:
	struct SettingFunctionButton:public Button{
	public:
		SettingFunctionButton(Vector2D buttonPos,Vector2D buttonVec,const std::string &buttonName,const std::function<void(EditActionSettings &)> &pushedCallback);
		~SettingFunctionButton()=default;
		void ButtonDraw(int font,int fillFlag)const;//ボタン自体を描画する
		bool JudgeInButton(Vector2D point)const;//ボタンの中にあるかの判定
		bool JudgeButtonPushed()const;//ボタンを押す動作がされたかを判定する
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時に行う処理

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
