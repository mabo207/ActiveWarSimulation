#ifndef DEF_GENERALPURPOSERESOURCE_H
#define DEF_GENERALPURPOSERESOURCE_H

#include<vector>

//�Q�[���̂����Ȃ��ʂł��p�ɂɗp������f�ނ��Ǘ�����N���X
//�C���N���[�h����΂ǂ̃\�[�X�R�[�h�ł��Ăяo����
struct GeneralPurposeResource{
private:

public:
	//�摜

	//�t�H���g
	static int popLargeFont;

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
