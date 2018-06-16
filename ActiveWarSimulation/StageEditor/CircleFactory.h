#ifndef DEF_CIRCLEFACTORY_H
#define DEF_CIRCLEFACTORY_H

#include"ShapeFactory.h"

//円を作るクラス
class CircleFactory:public ShapeFactory{
	//型・列挙体
public:
	struct CircleFactoryButton:public ShapeFactoryButton{
		CircleFactoryButton(Vector2D point,Vector2D vec);
		void ButtonDraw(int font,int fillFlag)const;//ボタンの描画
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時に行う処理
	};
	//定数

	//変数
protected:

	//関数
protected:

public:
	CircleFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor);
	~CircleFactory();

	std::shared_ptr<Shape> CreateShape(Vector2D point)const;
};


#endif // !DEF_CIRCLEFACTORY_H
#pragma once
