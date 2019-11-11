#ifndef DEF_REFLECTIONWORK_SELECTONE_H
#define DEF_REFLECTIONWORK_SELECTONE_H

#include"ReflectionWorkBase.h"
#include"Shape.h"
#include<vector>

namespace ReflectionWork{
	//どれか１つを選ぶワーク
	class SelectOne:public Base{
	public:
		SelectOne(const std::shared_ptr<const Shape> &answer,const std::vector<std::shared_ptr<const Shape>> &dummyList,const std::string &question);
		~SelectOne();

		//仮想関数のオーバーライド
		void Update();
		bool WorkClear()const;
		void WorkDraw()const;

	private:
		struct Component{
			Component(const std::shared_ptr<const Shape> &i_shape,bool i_correct):shape(i_shape),correct(i_correct){}
			~Component()=default;
			std::shared_ptr<const Shape> shape;//クリック箇所の形状
			bool correct;//正しいかどうか
		};

		std::vector<Component> m_choiceList;//選択肢一覧
		bool m_selected;//選択したかどうか
		bool m_correct;//正しく選べたか
	};
}

#endif // !DEF_REFLECTIONWORK_SELECTONE_H
