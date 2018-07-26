#ifndef DEF_RESEARCHSCENE_H
#define DEF_RESEARCHSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include"Unit.h"

//マップの情報を調べるモードの場面変数
class ResearchScene:public BattleSceneElement{
	//型・列挙体

	//定数
	static const int explainFontSize;

	//変数
protected:

	//ゲームに使用する変数
	std::shared_ptr<BattleSceneData> m_battleSceneData;//ゲーム内全体で使うデータ

	//調査モードで用いる変数
	const Unit *m_researchUnit;//情報を表示するユニット
	Vector2D m_pointerVec;//調べている場所

	const int m_palFont;//パラメータ表示のためのフォント
	const int m_nameFont;//名前表示のためのフォント
	const int m_explainFont;//説明文表示のためのフォント
	const int m_palBackPic;//パラメータ表示のための背景絵


	//関数
protected:
	void UpdatePointer();//調べる箇所の更新

	//仮想関数オーバーライド
	int thisCalculate();//0:直前場面に戻る 1~END-1:その場面に進む END:バトルパート終了
	void thisDraw()const;
	int UpdateNextScene(int index);//次場面の設定。indexに戻りたいが、実際に戻れるかは不明なので実際の戻り場所を示すint値を返し直す。基本的に返り値はindexに等しくなる。
	void ReturnProcess();//この場面に戻ってきた時の処理

public:
	ResearchScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~ResearchScene();

};


#endif // !DEF_RESEARCHSCENE_H
