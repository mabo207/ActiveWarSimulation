#ifndef DEF_GGEDITOR_H
#define DEF_GGEDITOR_H

#include<string>
#include<vector>
#include<memory>
#include"EditActionSettings.h"
#include"ToolsLib.h"
#include"ButtonHaving.h"
#include"Unit.h"

//ステージエディタ自身
class StageEditor {
	//列挙体・型
protected:
	
	//定数
public:

	//変数
protected:
	EditActionSettings m_actionSettings;//現在行おうとしている編集行為と編集データのまとめ

	//ボタン一覧
	std::vector<std::shared_ptr<ButtonHaving::Button>> m_buttons;
	
	//描画用データ
	int m_font;//フォント
	int m_mapPic;//背景グラフィック


	//関数
protected:
	//マウスを左クリックした時の動作
	void ProcessMapPush(int mouseX,int mouseY);//マップ画面を押した時の動作
	//常に行う編集行為
	void NonPressEdit(int mouseX,int mouseY);
	//ユニット情報の表示
	void DrawUnitInfo(const std::shared_ptr<const Unit> &punit,const Vector2D unitDrawPos)const;
	void DrawUnitInfo(const std::shared_ptr<const Unit> &punit,const Vector2D unitDrawPos,bool drawEditItem,size_t index)const;
	
public:
	//コンストラクタとデストラクタ
	StageEditor();
	~StageEditor();

	//毎ループ動作部分
	int Calculate();
	void Draw();

	//静的関数
private:
public:
	static int GetEditorSizeX();
	static int GetEditorSizeY();
};

#endif // !DEF_GGEDITOR_H
#pragma once
