#ifndef DEF_RECTANGLEFACTORY_H
#define DEF_RECTANGLEFACTORY_H

#include"ShapeFactory.h"

class RectangleFactory:public ShapeFactory{
	//型・列挙体
public:
	struct RectangleFactoryButton:public ShapeFactoryButton{
		RectangleFactoryButton(Vector2D point,Vector2D vec);
		void ButtonDraw(int font,int fillFlag)const;//ボタンの描画
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時に行う処理
	};
	//定数

	//変数
protected:

	//関数
protected:

public:
	RectangleFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor);
	~RectangleFactory();

	std::shared_ptr<Shape> CreateShape(Vector2D point)const;
};

#endif // !DEF_RECTANGLEFACTORY_H
#pragma once
