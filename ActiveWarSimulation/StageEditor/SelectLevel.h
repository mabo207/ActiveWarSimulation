#ifndef DEF_SELECTLEVEL_H
#define DEF_SELECTLEVEL_H

#include"ButtonHaving.h"
#include"StageLevel.h"

//読み込み・出力するステージレベルを選択する
class SelectLevel:public ButtonHaving{
public:
	struct SelectLevelButton:public Button{
	public:
		SelectLevelButton(Vector2D buttonPos,Vector2D buttonVec,StageLevel level);
		~SelectLevelButton()=default;
		void ButtonDraw(int font,int fillFlag)const;//ボタン自体を描画する
		bool JudgeInButton(Vector2D point)const;//ボタンの中にあるかの判定
		bool JudgeButtonPushed()const;//ボタンを押す動作がされたかを判定する
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時に行う処理

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