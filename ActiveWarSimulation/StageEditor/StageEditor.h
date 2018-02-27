#ifndef DEF_GGEDITOR_H
#define DEF_GGEDITOR_H

#include<string>
#include<vector>
#include<memory>
#include"EditActionSettings.h"
#include"ToolsLib.h"
#include"ButtonHaving.h"

//ステージエディタ自身
class StageEditor {
	//列挙体・型
protected:
	

	//定数
public:
	static const int mapSizeX,mapSizeY;//マップの表示部分の大きさ
	static const int leftUpPosX,leftUpPosY;//マップの左上の座標
	
	static const int buttonWidth,buttonHeight;//「動作」ボタン部分全体での横幅,縦幅
	static const int actButtonHeightNum=2,actButtonWidthNum=2;//「動作」ボタンの縦横の個数
	
	static const int shapeButtonWidth,shapeButtonHeight;//「図形設定」ボタン部分全体での横幅,縦幅
	static const int shapeButtonWidthNum,shapeButtonHeightNum;//「図形設定」ボタンの縦横の個数

	static const int posButtonWidth,posButtonHeight;//「位置設定」ボタン部分全体での横幅,縦幅
	static const int posButtonWidthNum,posButtonHeightNum;//「位置設定」ボタンの縦横の個数
	
	static const std::string actButtonStr[actButtonHeightNum*actButtonWidthNum];//「動作」ボタンに書く文字列

	static const int baseSize;//エディタで作られる物のサイズの基準の大きさ・基本単位

	//変数
protected:
	EditActionSettings m_actionSettings;//現在行おうとしている編集行為と編集データのまとめ

	//ボタン一覧
	std::vector<std::shared_ptr<ButtonHaving::Button>> m_buttons;
	
	//描画用データ
	int m_font;//フォント



	//関数
protected:
	//マウスを左クリックした時の動作
	void ProcessMapPush(int mouseX,int mouseY);//マップ画面を押した時の動作
	//常に行う編集行為
	void NonPressEdit(int mouseX,int mouseY);

public:
	//コンストラクタとデストラクタ
	StageEditor();
	~StageEditor();

	//毎ループ動作部分
	int Calculate();
	void Draw();

	//静的関数
private:
};

#endif // !DEF_GGEDITOR_H
#pragma once
