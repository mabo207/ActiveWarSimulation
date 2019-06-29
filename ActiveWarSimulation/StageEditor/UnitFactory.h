#ifndef DEF_UNITFACTORY_H
#define DEF_UNITFACTORY_H

#include"ShapeFactory.h"
#include"Unit.h"

//���j�b�g�𐶐�����
class UnitFactory:public ShapeFactory{
public:
	struct UnitFactoryButton:public ShapeFactoryButton{
		UnitFactoryButton(Vector2D point,Vector2D vec,Unit::Profession::Kind profession);
		void ButtonDraw(int font,int fillFlag)const;//�{�^���̕`��
		void PushedProcess(EditActionSettings &settings)const;//�{�^���������ꂽ���ɍs������
		
		const Unit::Profession::Kind m_profession;
	};

	UnitFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightColor,const Unit::Profession::Kind profession);
	~UnitFactory();
	std::shared_ptr<BattleObject> CreateObject(Vector2D point)const;

private:
	const Unit::Profession::Kind m_profession;
};

#endif // !DEF_UNITFACTORY_H