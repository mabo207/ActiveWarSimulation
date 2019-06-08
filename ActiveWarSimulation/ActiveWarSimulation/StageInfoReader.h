#ifndef DEF_STAGEINFOREADER_H
#define DEF_STAGEINFOREADER_H

//�ǂ̃V�[���ł��Ă�ł��ǂ���A�Ƃ����֐��E�N���X
//���̃\�����[�V�����Ɏg�p��������̂�Tools�t�H���_�ɓ�����Ȃ����A�F�X�ȉӏ��Ŏg�����������������ɏ����B

#include"StageLevel.h"

//"Stage/hoge/stageInfo.txt"����A����̒l�����o�����߂̃N���X�B
class StageInfoReader{
public:
	StageInfoReader(const std::string &dirName);
	~StageInfoReader();
	StageLevel GetLevel()const{
		return stageLevel;
	}
	std::string GetTitleName()const{
		return titleName;
	}

private:
	StageLevel stageLevel;
	std::string titleName;
};

#endif // !DEF_STAGEINFOREADER_H