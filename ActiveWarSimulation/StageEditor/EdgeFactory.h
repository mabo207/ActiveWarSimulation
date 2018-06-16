#ifndef DEF_EDGEFACTORY_H
#define DEF_EDGEFACTORY_H

#include"ShapeFactory.h"

class EdgeFactory:public ShapeFactory{
	//型・列挙体
public:
	struct EdgeFactoryButton:public ShapeFactoryButton{
		EdgeFactoryButton(Vector2D point,Vector2D vec);
		void ButtonDraw(int font,int fillFlag)const;//ボタンの描画
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時に行う処理
	};
	//定数
protected:
	static const Vector2D baseVec;//生成される線分のベクトルの初期値

	//変数
protected:

	//関数
protected:

public:
	EdgeFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor);
	~EdgeFactory();

	std::shared_ptr<Shape> CreateShape(Vector2D point)const;
};

#endif // !DEF_EDGEFACTORY_H

#pragma once
