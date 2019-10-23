#ifndef DEF_WHOLEREFLECTIONINFO_H
#define DEF_WHOLEREFLECTIONINFO_H

#include"LogElement.h"

struct WholeReflectionInfo{
	//リフレクションに必要な情報群
	//関数
	WholeReflectionInfo(const std::pair<int,std::shared_ptr<const LogElement>> &goodLog,const std::pair<int,std::shared_ptr<const LogElement>> &badLog)
		:m_goodLog(goodLog)
		,m_badLog(badLog)
	{}
	~WholeReflectionInfo()=default;
	//変数
	std::pair<int,std::shared_ptr<const LogElement>> m_goodLog;//良い動きをしたとされる時の評価とログ
	std::pair<int,std::shared_ptr<const LogElement>> m_badLog;//良くない動きをしたとされる時の評価とログ
};

#endif // !DEF_WHOLEREFLECTIONINFO_H
