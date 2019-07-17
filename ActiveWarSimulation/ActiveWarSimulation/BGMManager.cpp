#include"BGMManager.h"
#include"DxLib.h"

//---------------BGMManager----------------
std::optional<BGMManager> BGMManager::s_instance;

void BGMManager::PlayWithCopy(const Resource::BGM &originBGM){
	if(!(m_nowPlayingBGM==originBGM)){
		//������BGM�𒆒f�E���
		Stop();
		//�V����BGM��m_nowPlayeingBGM�ɃR�s�[���čĐ�
		m_nowPlayingBGM=originBGM.Copy();
		m_nowPlayingBGM.SetAndPlay(DX_PLAYTYPE_LOOP,TRUE);
	}
}

void BGMManager::Stop(){
	//������BGM�𒆒f�E���
	m_nowPlayingBGM.Stop();
	m_nowPlayingBGM.Delete();
	//m_nowPlayingBGM�ɍĐ��ł��Ȃ�BGM�����Ă���
	m_nowPlayingBGM=Resource::BGM::errorObject;
}

Resource::BGM BGMManager::GetCopiedBGM()const{
	return m_nowPlayingBGM.Copy();
}

void BGMManager::Init(){
	s_instance=BGMManager();
}

void BGMManager::Release(){
	s_instance.reset();
}

BGMManager::BGMManager()
	:m_nowPlayingBGM(Resource::BGM::errorObject)
{}

BGMManager::~BGMManager(){
	Stop();
}
