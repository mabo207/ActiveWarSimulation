#ifndef DEF_INITLOG_H
#define DEF_INITLOG_H

#include"LogElement.h"

//�o�g���̍ŏ��̃��j�b�g�̏�Ԃ���郍�O
class InitLog:public LogElement{
public:
	InitLog(const BattleSceneData * const battleData)
		:LogElement(LogKind::e_init,battleData){}
	virtual ~InitLog(){}
	//���z�֐��̃I�[�o�[���C�h
	void OutputLog(std::ofstream &ofs,const std::map<const Unit *,size_t> &unitPtrToIndex,const char splitter,const char beginer,const char ender)const;
};

#endif // !DEF_INITLOG_H