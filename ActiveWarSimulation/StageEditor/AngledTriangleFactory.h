#ifndef DEF_ANGLEDTRIANGLEFACTORY_H
#define DEF_ANGLEDTRIANGLEFACTORY_H

#include"ShapeFactory.h"

//直角三角形を作るクラス
class AngledTriangleFactory:public ShapeFactory{
	//型・列挙体
public:
	struct AngledTriangleFactoryButton:public ShapeFactoryButton{
		AngledTriangleFactoryButton(Vector2D point,Vector2D vec);
		void ButtonDraw(int font,int fillFlag)const;//ボタンの描画
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時に行う処理
	};
	//定数

	//変数
protected:

	//関数
protected:

public:
	AngledTriangleFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor);
	~AngledTriangleFactory();

	std::shared_ptr<Shape> CreateShape(Vector2D point)const;
};


#endif // !DEF_ANGLEDTRIANGLEFACTORY_H
#pragma once
