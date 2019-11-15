#ifndef DEF_REFLECTIONWORK_READEXPLANATION_H
#define DEF_REFLECTIONWORK_READEXPLANATION_H

#include"ReflectionWorkBase.h"
#include<vector>
#include"Shape.h"

namespace ReflectionWork{
	//�����ǂރ��[�N
	class ReadExplanation:public Base{
	public:
		ReadExplanation(const std::vector<std::pair<const Shape *,unsigned int>> &assistShapeList
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
		const std::vector<std::pair<const Shape *,unsigned int>> m_assistShapeList;//����̕⏕���s���}�`�̕`����ꗗ
		const std::shared_ptr<const Base> m_beforeWork;//���O�̃��[�N
	};
}

#endif // !DEF_REFLECTIONWORK_READEXPLANATION_H
