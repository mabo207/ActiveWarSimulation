#ifndef DEF_INITLOG_H
#define DEF_INITLOG_H

#include"LogElement.h"

//�o�g���̍ŏ��̃��j�b�g�̏�Ԃ���郍�O
class InitLog:public LogElement{
public:
	InitLog(const BattleSceneData * const battleData)
		:LogElement(LogKind::e_init,battleData){}
	virtual ~InitLog(){}
};

#endif // !DEF_INITLOG_H