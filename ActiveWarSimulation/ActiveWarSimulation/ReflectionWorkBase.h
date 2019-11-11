#ifndef DEF_REFLECTIONWORKBASE_H
#define DEF_REFLECTIONWORKBASE_H

#include<string>

//リフレクション活動の際のワークを定義する
namespace ReflectionWork{
	class Base{
	public:
		enum class Kind{
			e_lineDraw
			,e_clickObject
		};

		virtual void Update()=0;
		virtual bool WorkClear()const=0;
		virtual void WorkDraw()const=0;
		virtual ~Base()=default;
		Kind GetKind()const{
			return m_kind;
		}
		const std::string GetQuestion()const{
			return m_question;
		}

	protected:
		explicit Base(Kind kind,const std::string &question):m_kind(kind),m_question(question){}

		const Kind m_kind;
		const std::string m_question;
	};
}

#endif // !DEF_REFLECTIONWORK_H
