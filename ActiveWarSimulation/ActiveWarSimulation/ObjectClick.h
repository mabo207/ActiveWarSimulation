#ifndef DEF_REFLECTIONWORK_OBJECTCLICK_H
#define DEF_REFLECTIONWORK_OBJECTCLICK_H

#include"ReflectionWorkBase.h"
#include"Shape.h"
#include<vector>

namespace ReflectionWork{
	//指定されたオブジェクトをクリックするワーク
	class ObjectClick:public Base{
	public:
		ObjectClick(const std::vector<std::shared_ptr<const Shape>> &shapeList);
		~ObjectClick();

		//仮想関数のオーバーライド
		void Update();
		bool WorkClear()const;
		void WorkDraw()const;

	private:
		//型
		struct ObjectInfo{
			//オブジェクトのクリック情報
			const std::shared_ptr<const Shape> shape;
			bool clicked;//クリックされたかどうか
			explicit ObjectInfo(const std::shared_ptr<const Shape> &i_shape):shape(i_shape),clicked(false){}
			~ObjectInfo()=default;
		};

		//変数
		std::vector<ObjectInfo> m_objectList;
	};
}


#endif // !DEF_REFLECTIONWORK_OBJECTCLICK_H

