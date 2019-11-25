#ifndef DEF_REFLECTIONWORK_SELECTONE_H
#define DEF_REFLECTIONWORK_SELECTONE_H

#include"ReflectionWorkBase.h"
#include"Shape.h"
#include<vector>

namespace ReflectionWork{
	//�ǂꂩ�P��I�ԃ��[�N
	class SelectOne:public Base{
	public:
		SelectOne(const std::shared_ptr<const Shape> &answer,const std::vector<std::shared_ptr<const Shape>> &dummyList,const std::string &question);
		~SelectOne();

		//���z�֐��̃I�[�o�[���C�h
		void Update();
		bool WorkClear()const;
		void WorkDraw()const;

	private:
		struct Component{
			Component(const std::shared_ptr<const Shape> &i_shape,bool i_correct):shape(i_shape),correct(i_correct){}
			~Component()=default;
			std::shared_ptr<const Shape> shape;//�N���b�N�ӏ��̌`��
			bool correct;//���������ǂ���
		};

		std::vector<Component> m_choiceList;//�I�����ꗗ
		bool m_selected;//�I���������ǂ���
		bool m_correct;//�������I�ׂ���
	};
}

#endif // !DEF_REFLECTIONWORK_SELECTONE_H
