#ifndef DEF_REFLECTIONWORK_READEXPLANATION_H
#define DEF_REFLECTIONWORK_READEXPLANATION_H

#include"ReflectionWorkBase.h"
#include<vector>
#include"Shape.h"

namespace ReflectionWork{
	//解説を読むワーク
	class ReadExplanation:public Base{
	public:
		//理解を補助するための図形の描画情報
		struct AssistShapeInfo{
			//関数
			AssistShapeInfo(const std::shared_ptr<const Shape> &i_shape,unsigned int i_color,int i_alpha=opacityAlpha)
				:shape(i_shape),color(i_color),alpha(i_alpha){}
			void Draw()const;
			//変数
			std::shared_ptr<const Shape> shape;
			unsigned int color;
			int alpha;
			static const int opacityAlpha=255;//不透明を示すalpha
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
		const std::vector<AssistShapeInfo> m_assistShapeList;//解説の補助を行う図形の描画情報一覧
		const std::shared_ptr<const Base> m_beforeWork;//直前のワーク
	};
}

#endif // !DEF_REFLECTIONWORK_READEXPLANATION_H
