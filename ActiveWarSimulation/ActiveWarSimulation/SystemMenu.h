#ifndef DEF_SYSTEMMENU_H
#define DEF_SYSTEMMENU_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include<memory>

class SystemMenu:public BattleSceneElement{
	//型・列挙体

	//定数
protected:
	static const size_t indexCount=2;
	static const int contentX[indexCount],contentY[indexCount];
	static const int contentWidth,contentHeight;
	static const int sentenceX,sentenceY;
	static const std::string contentStr[indexCount];

	//変数
protected:
	//ゲームデータ(描画に必要)
	std::shared_ptr<const BattleSceneData> m_battleSceneData;
	
	//項目選択
	size_t m_index;
	Vector2D m_beforeFrameMousePos;//前フレームのマウスの位置

	//画像描画
	const int m_systemBoard;//背景描画
	const int m_contentFont;

	//関数
protected:
	int thisCalculate();//0:直前場面に戻る 1~END-1:その場面に進む END:バトルパート終了
	void thisDraw()const;
	int UpdateNextScene(int index);//次場面の設定。indexに戻りたいが、実際に戻れるかは不明なので実際の戻り場所を示すint値を返し直す。基本的に返り値はindexに等しくなる。
	void ReturnProcess();//この場面に戻ってきた時の処理

public:
	SystemMenu(std::shared_ptr<BattleSceneData> battleSceneData);
	~SystemMenu();

};

#endif // !DEF_SYSTEMMENU_H
