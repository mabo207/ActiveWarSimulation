#ifndef DEF_EDGEFACTORY_H
#define DEF_EDGEFACTORY_H

#include"ShapeFactory.h"

class EdgeFactory:public ShapeFactory{
	//�^�E�񋓑�
public:
	struct EdgeFactoryButton:public ShapeFactoryButton{
		EdgeFactoryButton(Vector2D point,Vector2D vec);
		void ButtonDraw(int font,int fillFlag)const;//�{�^���̕`��
		void PushedProcess(EditActionSettings &settings)const;//�{�^���������ꂽ���ɍs������
	};
	//�萔
protected:
	static const Vector2D baseVec;//�������������̃x�N�g���̏����l

	//�ϐ�
protected:

	//�֐�
protected:

public:
	EdgeFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor);
	~EdgeFactory();

	std::shared_ptr<Shape> CreateShape(Vector2D point)const;
};

#endif // !DEF_EDGEFACTORY_H

#pragma once
