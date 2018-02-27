#ifndef DEF_EDITACTION_H
#define DEF_EDITACTION_H

#include"ButtonHaving.h"

//編集行為を表すクラスのインターフェース
class EditAction:public ButtonHaving{
	//型・列挙体
public:
	//ボタンの基底クラス
	struct EditActionButton:public Button{
		//このクラスの図形は全て長方形
		//変数
		Vector2D m_point;//左上の頂点の座標
		Vector2D m_vec;//対角線のベクトル
		std::string m_str;//ボタン内に描画する文字列
		//関数
		EditActionButton(Vector2D point,Vector2D vec,const std::string &str);
		void ButtonDraw(int font,int fillFlag)const;//ボタン自体を描画する
		bool JudgeInButton(Vector2D point)const;//ボタンの中にあるかの判定
		bool JudgeButtonPushed()const;//ボタンを押す動作がされたかを判定する
		//void PushedProcess(EditActionSettings &settings)const;//継承先で実装
	};
	//編集行為の際の点の位置決定の方針
	enum PosSetKind{
		NONEDIT//決して手を加えない(EditRemove,EditMove,EditResizeの対象選択時)
		,BASENONEXIST//編集時に基準点が存在しない(EditMove,EditPutの設置位置決定時)(EditActionSettings::m_pBattleObjectがnullptrであってはいけない)
		,BASEEXIST//編集時に基準点が存在する(EditResize,EditPutのオブジェクトサイズ変更時)(EditActionSettings::m_pBattleObjectがnullptrであってはいけない)
	};

	//定数

	//変数
protected:
	int m_buttonX,m_buttonY,m_buttonDX,m_buttonDY;//ボタンの位置と大きさ
	unsigned int m_pushedColor;//ボタンが押されている時の色
	
	//関数
public:
	EditAction(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor)
		:m_buttonX(buttonX),m_buttonY(buttonY),m_buttonDX(buttonDX),m_buttonDY(buttonDY),m_pushedColor(pushedColor){}
	virtual ~EditAction() {}
	virtual void VNonPressEditing(Vector2D point,EditActionSettings &settings)const=0;//マウスを押していない時も編集を行う編集行為について、そのような編集行為を行う
	virtual void VProcessAction(Vector2D point,EditActionSettings &settings)const=0;//マップ上の点の座標を取得しsettingsに対し編集行為を実行する
	void DrawPushedButton()const;//該当するボタンが押されているように表示する関数
	virtual PosSetKind VGetPosSetKind(const EditActionSettings &settings)const=0;//編集状況に依って返す値が異なるため
};


#endif // !DEF_EDITACTION_H
#pragma once
