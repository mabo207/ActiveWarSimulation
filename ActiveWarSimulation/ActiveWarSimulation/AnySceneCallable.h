#ifndef DEF_ANYSCENECALLABLE_H
#define DEF_ANYSCENECALLABLE_H

//�ǂ̃V�[���ł��Ă�ł��ǂ���A�Ƃ����֐��E�N���X
//���̃\�����[�V�����Ɏg�p��������̂�Tools�t�H���_�ɓ�����Ȃ����A�F�X�ȉӏ��Ŏg�����������������ɏ����B

#include"ToolsLib.h"

//"Stage/hoge/stageInfo.txt"����A����̒l�����o�����߂̃N���X�B
class StageInfoReader{
public:
	StageInfoReader(const std::string &dirName);
	~StageInfoReader();
	int GetLevel()const{
		return stageLevel;
	}
	std::string GetTitleName()const{
		return titleName;
	}

private:
	int stageLevel;
	std::string titleName;
};

#endif // !DEF_ANYSCENECALLABLE_H