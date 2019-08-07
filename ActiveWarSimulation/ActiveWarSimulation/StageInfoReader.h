#ifndef DEF_STAGEINFOREADER_H
#define DEF_STAGEINFOREADER_H

//�ǂ̃V�[���ł��Ă�ł��ǂ���A�Ƃ����֐��E�N���X
//���̃\�����[�V�����Ɏg�p��������̂�Tools�t�H���_�ɓ�����Ȃ����A�F�X�ȉӏ��Ŏg�����������������ɏ����B

#include<string>
#include"ToolsLib.h"

//"Stage/hoge/stageInfo.txt"����A����̒l�����o�����߂̃N���X�B
class StageInfoReader{
public:
	StageInfoReader(const std::string &dirName);
	~StageInfoReader();
	std::string GetTitleName()const{
		return titleName;
	}
	Vector2D GetPos()const{
		return pos;
	}
	std::string GetBgmInfoFileName()const{
		return bgmFolderName;
	}

private:
	std::string titleName;
	Vector2D pos;
	std::string bgmFolderName;
};

#endif // !DEF_STAGEINFOREADER_H