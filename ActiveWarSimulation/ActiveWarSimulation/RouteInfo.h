#ifndef DEF_ROUTEINFO_H
#define DEF_ROUTEINFO_H

#include"ToolsLib.h"

//ユニットの通った経路の情報
struct RouteInfo{
	const Vector2D pos;
	const float OP;
	RouteInfo(Vector2D i_pos,float i_OP):pos(i_pos),OP(i_OP){}
};

#endif // !DEF_ROUTEINFO_H
