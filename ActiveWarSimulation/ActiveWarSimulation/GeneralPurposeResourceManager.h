#ifndef DEF_GENERALPURPOSERESOURCEMANAGER_H
#define DEF_GENERALPURPOSERESOURCEMANAGER_H

#include<vector>

//�Q�[���̂����Ȃ��ʂł��p�ɂɗp������f�ނ��Ǘ�����N���X
//�C���N���[�h����΂ǂ̃\�[�X�R�[�h�ł��Ăяo����
struct GeneralPurposeResourceManager{
private:

public:
	//�摜

	//�t�H���g

	//��
	static int decideSound;
	static int cancelSound;
	static int selectSound;

	//���[�_�[�֐�
	static void LoadResource();//main�֐��ȊO�ŌĂяo����Ă͂����Ȃ�
	//����֐�
	static void ReleaseResource();//main�֐��ȊO�ŌĂяo����Ă͂����Ȃ�
};


#endif // !DEF_GENERALPURPOSEEFFECTSOUNDMANAGER_H
