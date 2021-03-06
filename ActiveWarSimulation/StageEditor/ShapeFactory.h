#ifndef DEF_SHAPEFACTORY_H

#include<memory>
#include"ButtonHaving.h"
#include"EditPut.h"
#include"BattleObject.h"

//当たり判定図形を作るクラス
class ShapeFactory:public ButtonHaving{
	//型・列挙体
public:
	struct ShapeFactoryButton:public Button{
		//当たり判定図形を設定するボタン。長方形の半透明描画と当たり判定の図形の描画で表現。
		//変数
		Vector2D m_point;//左上の座標
		Vector2D m_vec;//対角線
		//関数
		ShapeFactoryButton(Vector2D point,Vector2D vec);
		//void ButtonDraw(int font,int fillFlag)const;//継承先で実装
		bool JudgeInButton(Vector2D point)const;//ボタンの中にあるかの判定
		bool JudgeButtonPushed()const;//ボタンを押す動作がされたかを判定する
		//void PushedProcess(EditActionSettings &settings)const;//継承先で実装
	};

	//定数

	//変数
protected:
	//ボタンのライトアップのための情報
	Vector2D m_buttonPos,m_buttonSize;//ボタンの位置と大きさ
	unsigned int m_buttonLightColor;//ライトアップの色

	//関数
protected:

public:
	ShapeFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor);
	virtual ~ShapeFactory();
	void DrawPushedButtonLight()const;//現在選択されている当たり判定図形に相当するボタンの背後に長方形を描画する
	virtual std::shared_ptr<BattleObject> CreateObject(Vector2D point)const=0;//当たり判定図形を作り出す
	virtual EditPut::PosSetKind VPutAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings);//EditPutのクリックの際に行う処理(デフォルトは位置確定と大きさ設定を入れ替えるやり方)
	virtual void VPutNotPressAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings)const;//EditPutの非クリックの際に行う処理(デフォルトはResize()をしていくだけ)
	virtual void FactoryDraw(const Vector2D adjust,const EditActionSettings &settings)const;//図形配置がしやすいような描画をする。多くは設置前のBattleObjectを置くが、Polygonは別処理をする。
};


#endif // !DEF_SHAPEFACTORY_H
#pragma once
