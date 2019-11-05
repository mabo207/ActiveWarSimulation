#ifndef DEF_REFLECTIONWORK_OBJECTCLICK_H
#define DEF_REFLECTIONWORK_OBJECTCLICK_H

#include"ReflectionWorkBase.h"
#include"Shape.h"
#include<vector>

namespace ReflectionWork{
	//�w�肳�ꂽ�I�u�W�F�N�g���N���b�N���郏�[�N
	class ObjectClick:public Base{
	public:
		ObjectClick(const std::vector<std::shared_ptr<const Shape>> &shapeList);
		~ObjectClick();

		//���z�֐��̃I�[�o�[���C�h
		void Update();
		bool WorkClear()const;
		void WorkDraw()const;

	private:
		//�^
		struct ObjectInfo{
			//�I�u�W�F�N�g�̃N���b�N���
			const std::shared_ptr<const Shape> shape;
			bool clicked;//�N���b�N���ꂽ���ǂ���
			explicit ObjectInfo(const std::shared_ptr<const Shape> &i_shape):shape(i_shape),clicked(false){}
			~ObjectInfo()=default;
		};

		//�ϐ�
		std::vector<ObjectInfo> m_objectList;
	};
}


#endif // !DEF_REFLECTIONWORK_OBJECTCLICK_H

