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
protected:
	static const float pointCapacity;

	//変数
protected:
	//多角形構成のための変数
	std::vector<Vector2D> m_pointVec;

	//関数
protected:

public:
	PolygonFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor);
	~PolygonFactory();

	std::shared_ptr<BattleObject> CreateObject(Vector2D point)const;
	EditPut::PosSetKind VPutAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings);
	void VPutNotPressAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings)const;//EditPutの非クリックの際に行う処理(デフォルトはResize()をしていくだけ)
	void FactoryDraw(const Vector2D adjust,const EditActionSettings &settings)const;//図形配置がしやすいような描画をする。
};

#endif // !DEF_POLYGONFACTORY_H
