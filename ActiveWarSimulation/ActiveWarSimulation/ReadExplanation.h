#ifndef DEF_REFLECTIONWORK_READEXPLANATION_H
#define DEF_REFLECTIONWORK_READEXPLANATION_H

#include"ReflectionWorkBase.h"
#include<vector>
#include"Shape.h"

namespace ReflectionWork{
	//�����ǂރ��[�N
	class ReadExplanation:public Base{
	public:
		//������⏕���邽�߂̐}�`�̕`����
		struct AssistShapeInfo{
			//�֐�
			AssistShapeInfo(const std::shared_ptr<const Shape> &i_shape,unsigned int i_color,int i_alpha=opacityAlpha)
				:shape(i_shape),color(i_color),alpha(i_alpha){}
			void Draw()const;
			//�ϐ�
			std::shared_ptr<const Shape> shape;
			unsigned int color;
			int alpha;
			static const int opacityAlpha=255;//�s����������alpha
		};

		ReadExplanation(const std::vector<AssistShapeInfo> &assistShapeList
			,const std::shared_ptr<const Base> &beforeWork
			,const std::string &question)
			:Base(Kind::e_readExplanation,question)
			,m_assistShapeList(assistShapeList)
			,m_beforeWork(beforeWork)
		{}
		~ReadExplanation()=default;
		void Update();
		bool WorkClear()const;
		void WorkDraw()const;
	private:
		const std::vector<AssistShapeInfo> m_assistShapeList;//����̕⏕���s���}�`�̕`����ꗗ
		const std::shared_ptr<const Base> m_beforeWork;//���O�̃��[�N
	};
}

#endif // !DEF_REFLECTIONWORK_READEXPLANATION_H
