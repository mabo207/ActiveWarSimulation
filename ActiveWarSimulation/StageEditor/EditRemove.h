#ifndef DEF_EDITMOVE_H

#include"EditAction.h"

//オブジェクトを取り除く操作を記述したクラス
class EditRemove:public EditAction{
	//型・列挙体
public:
	struct EditRemoveButton:public EditActionButton{
		EditRemoveButton(Vector2D point,Vector2D vec);
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時の処理
	};

	//定数

	//変数
protected:

	//関数
protected:

public:
	EditRemove(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor);
	~EditRemove(){}

	void VNonPressEditing(Vector2D point,EditActionSettings &settings)const;//マウスを押していない時も編集を行う編集行為について、そのような編集行為を行う
	void VProcessAction(Vector2D point,EditActionSettings &settings)const;
	PosSetKind VGetPosSetKind(const EditActionSettings &settings)const;//編集状況に依って返す値が異なるため
};

#endif // !DEF_EDITMOVE_H
