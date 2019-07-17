#ifndef DEF_BGMMANAGER_H
#define DEF_BGMMANAGER_H

#include"BGM.h"
#include<optional>

//BGM�̍Đ����Ǘ�����N���X
//�C���X�^���X�͂P�ɂ������̂ŁAstatic�ȐÓI�����o�������A�����ɃA�N�Z�X����
class BGMManager{
public:
	//�Đ����̂��̂ƈقȂ�ꍇ�AoriginBGM��Copy���āACopy�������̂��Đ�����
	void PlayWithCopy(const Resource::BGM &originBGM);
	//�Đ�����BGM���~�߁Am_nowPlayingBGM���������
	void Stop();
	//�Đ�����BGM�̏����R�s�[���Ď擾����
	Resource::BGM GetCopiedBGM()const;
	//�f�X�g���N�^
	~BGMManager();

	//�C���X�^���X�������E���
	static void Init();
	static void Release();
	
	//�C���X�^���X�͂P�ȉ��������݂���悤�ɂ���(Dxlib�̏�������ɃC���X�^���X���쐬���ADxlib�̉���O�ɃC���X�^���X�����������)
	static std::optional<BGMManager> s_instance;

private:
	BGMManager();

	Resource::BGM m_nowPlayingBGM;//�Đ�����BGM
};

#endif // !DEF_BGMMANAGER_H
