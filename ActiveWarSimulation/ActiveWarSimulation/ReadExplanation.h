#ifndef DEF_REFLECTIONWORK_READEXPLANATION_H
#define DEF_REFLECTIONWORK_READEXPLANATION_H

#include"ReflectionWorkBase.h"
#include<vector>
#include"Shape.h"

namespace ReflectionWork{
	//解説を読むワーク
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
		const std::vector<std::pair<const Shape *,unsigned int>> m_assistShapeList;//解説の補助を行う図形の描画情報一覧
		const std::shared_ptr<const Base> m_beforeWork;//直前のワーク
	};
}

#endif // !DEF_REFLECTIONWORK_READEXPLANATION_H
