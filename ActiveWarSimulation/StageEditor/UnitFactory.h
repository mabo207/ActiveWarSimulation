#ifndef DEF_UNITFACTORY_H
#define DEF_UNITFACTORY_H

#include"ShapeFactory.h"
#include"Unit.h"

//ユニットを生成する
class UnitFactory:public ShapeFactory{
public:
	struct UnitFactoryButton:public ShapeFactoryButton{
		UnitFactoryButton(Vector2D point,Vector2D vec,Unit::Profession::Kind profession);
		void ButtonDraw(int font,int fillFlag)const;//ボタンの描画
		void PushedProcess(EditActionSettings &settings)const;//ボタンが押された時に行う処理
		
		const Unit::Profession::Kind m_profession;
	};

	UnitFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightColor,const Unit::Profession::Kind profession);
	~UnitFactory();
	std::shared_ptr<BattleObject> CreateObject(Vector2D point)const;
	EditPut::PosSetKind VPutAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings);//大きさ調整できないようにする
	
private:
	const Unit::Profession::Kind m_profession;
};

#endif // !DEF_UNITFACTORY_H