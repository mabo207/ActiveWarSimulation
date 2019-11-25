#ifndef DEF_REFLECTIONWORK_LINEDRAW_H
#define DEF_REFLECTIONWORK_LINEDRAW_H

#include"ReflectionWorkBase.h"
#include"Edge.h"
#include<vector>

namespace ReflectionWork{
	//指定された線を引くワーク
	class LineDraw:public Base{
	public:
		LineDraw(const std::vector<Edge> &edgeList,const std::string &question);
		~LineDraw();

		//仮想関数のオーバーライド
		void Update();
		bool WorkClear()const;
		void WorkDraw()const;

	private:
		struct LineDrawInfo{
			//直線とその直線の入力が終わったかの情報をまとめるクラス
			Edge edge;
			bool drawInfo;//true:入力した false:まだ入力していない
			LineDrawInfo(const Edge &i_edge):LineDrawInfo(i_edge,false){}
			LineDrawInfo(const Edge &i_edge,const bool i_drawInfo):edge(i_edge),drawInfo(i_drawInfo){}
		};

		std::vector<LineDrawInfo> m_lineList;//指定した線一覧と入力終了情報
		bool m_inputing;//入力中かどうか
		size_t m_inputIndex;//どの線を入力しているか
		bool m_inputStart;//true:startを始点に線を引いている false:end始点
	};
}

#endif // !DEF_REFLECTIONWORK_LINEDRAW_H
