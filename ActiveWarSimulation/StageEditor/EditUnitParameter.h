#ifndef DEF_EDITUNITPARAMETER_H
#define DEF_EDITUNITPARAMETER_H

#include"EditAction.h"
#include"Unit.h"

//ユニットを選択して、パラメータを編集できるようにする編集操作
class EditUnitParameter:public EditAction{
	//型・列挙体
public:
	struct EditUnitParameterButton:public EditActionButton{
		EditUnitParameterButton(Vector2D point,Vector2D vec);
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時の処理
	};

	//定数
	const static size_t editItemNum;//編集項目数(増やしたらStageEditorの描画も変えてあげる)

	//変数
protected:
	size_t m_editIndex;//編集項目
	std::shared_ptr<Unit> m_editResult;//編集結果

	//関数
public:
	EditUnitParameter(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor);
	~EditUnitParameter();

	void VNonPressEditing(Vector2D point,EditActionSettings &settings)const;//マウスを押していない時も編集を行う編集行為について、そのような編集行為を行う
	void VProcessAction(Vector2D point,EditActionSettings &settings);//編集行為を実行する
	PosSetKind VGetPosSetKind(const EditActionSettings &settings)const;//編集状況に依って返す値が異なるため
	size_t GetEditIndex()const{
		return m_editIndex;
	}
	std::shared_ptr<Unit> GetEditResult()const{
		return m_editResult;
	}
	void EditParameter(bool up,bool down,bool left,bool right);
};

#endif // !DEF_EDITUNITPARAMETER_H