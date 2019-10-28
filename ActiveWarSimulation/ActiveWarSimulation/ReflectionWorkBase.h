#ifndef DEF_REFLECTIONWORKBASE_H
#define DEF_REFLECTIONWORKBASE_H

//リフレクション活動の際のワークを定義する
namespace ReflectionWork{
	class Base{
	public:
		enum class Kind{
			e_lineDraw
			,e_findObject
		};

		virtual void Update()=0;
		virtual bool WorkClear()const=0;
		virtual void WorkDraw()const=0;
		virtual ~Base()=default;
		Kind GetKind()const{
			return m_kind;
		}

	protected:
		explicit Base(Kind kind):m_kind(kind){}

		const Kind m_kind;
	};
}

#endif // !DEF_REFLECTIONWORK_H
