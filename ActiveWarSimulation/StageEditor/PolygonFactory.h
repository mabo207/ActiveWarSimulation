#ifndef DEF_POLYGONFACTORY_H
#define DEF_POLYGONFACTORY_H

#include"ShapeFactory.h"

class PolygonFactory:public ShapeFactory{
	//型・列挙体
public:
	struct PolygonFactoryButton:public ShapeFactoryButton{
		PolygonFactoryButton(Vector2D point,Vector2D vec);
		void ButtonDraw(int font,int fillFlag)const;//ボタンの描画
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時に行う処理
	};
	//定数

	//変数
protected:

	//関数
protected:

public:
	PolygonFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor);
	~PolygonFactory();

	std::shared_ptr<Shape> CreateShape(Vector2D point)const;
	EditPut::PosSetKind VPutAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings)const;
	void VPutNotPressAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings)const;//EditPutの非クリックの際に行う処理(デフォルトはResize()をしていくだけ)
};

#endif // !DEF_POLYGONFACTORY_H
