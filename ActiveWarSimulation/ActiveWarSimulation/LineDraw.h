#ifndef DEF_REFLECTIONWORK_LINEDRAW_H
#define DEF_REFLECTIONWORK_LINEDRAW_H

#include"ReflectionWorkBase.h"
#include"Edge.h"
#include<vector>

namespace ReflectionWork{
	//�w�肳�ꂽ�����������[�N
	class LineDraw:public Base{
	public:
		LineDraw(const std::vector<Edge> &edgeList,const std::string &question);
		~LineDraw();

		//���z�֐��̃I�[�o�[���C�h
		void Update();
		bool WorkClear()const;
		void WorkDraw()const;

	private:
		struct LineDrawInfo{
			//�����Ƃ��̒����̓��͂��I��������̏����܂Ƃ߂�N���X
			Edge edge;
			bool drawInfo;//true:���͂��� false:�܂����͂��Ă��Ȃ�
			LineDrawInfo(const Edge &i_edge):LineDrawInfo(i_edge,false){}
			LineDrawInfo(const Edge &i_edge,const bool i_drawInfo):edge(i_edge),drawInfo(i_drawInfo){}
		};

		std::vector<LineDrawInfo> m_lineList;//�w�肵�����ꗗ�Ɠ��͏I�����
		bool m_inputing;//���͒����ǂ���
		size_t m_inputIndex;//�ǂ̐�����͂��Ă��邩
		bool m_inputStart;//true:start���n�_�ɐ��������Ă��� false:end�n�_
	};
}

#endif // !DEF_REFLECTIONWORK_LINEDRAW_H
