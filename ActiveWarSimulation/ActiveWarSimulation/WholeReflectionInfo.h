#ifndef DEF_WHOLEREFLECTIONINFO_H
#define DEF_WHOLEREFLECTIONINFO_H

#include"LogElement.h"

struct WholeReflectionInfo{
	//���t���N�V�����ɕK�v�ȏ��Q
	//�֐�
	WholeReflectionInfo(const std::pair<int,std::shared_ptr<const LogElement>> &goodLog,const std::pair<int,std::shared_ptr<const LogElement>> &badLog)
		:m_goodLog(goodLog)
		,m_badLog(badLog)
	{}
	~WholeReflectionInfo()=default;
	//�ϐ�
	std::pair<int,std::shared_ptr<const LogElement>> m_goodLog;//�ǂ������������Ƃ���鎞�̕]���ƃ��O
	std::pair<int,std::shared_ptr<const LogElement>> m_badLog;//�ǂ��Ȃ������������Ƃ���鎞�̕]���ƃ��O
};

#endif // !DEF_WHOLEREFLECTIONINFO_H
