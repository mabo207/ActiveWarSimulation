#ifndef DEF_CLEARSTAGEDATA_H
#define DEF_CLEARSTAGEDATA_H

#include<string>
#include<set>

//�N���A�X�e�[�W�̈ꗗ�f�[�^
class ClearStageData{
public:
	ClearStageData();
	~ClearStageData()=default;
	bool JudgeClear(const std::string &stageDirName)const;//stageDirName���N���A���Ă��邩�ǂ���
	void BecomeClear(const std::string &stageDirName);//stageDirName���N���A�������̂Ƃ���
	bool Save()const;//���݂�m_clearDir���Z�[�u����

protected:
	std::set<std::string> m_clearDir;//�N���A�����X�e�[�W�̃f�B���N�g���ꗗ
};

#endif // !DEF_CLEARSTAGEDATA_H
